<?php
/**
 * @file
 * Availability Calendar: code to edit the calendar using a month based widget.
 *
 * @author Erwin Derksen (http://drupal.org/user/750928)
 */

module_load_include('inc', 'availability_calendar', 'availability_calendar');

/**
 * Defines the form elements to edit this field.
 *
 * Called by our implementation of hook_field_widget_form(). Parameters are as
 * passed to hook_field_widget_form(). Return is what hook_field_widget_form()
 * should return.
 *
 * @return array
 *   Form elements to edit this field on an entity edit form.
 */
function availability_calendar_field_widget_month_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  static $new_cid_count = 0;
  $item = isset($items[$delta]) ? $items[$delta] : NULL;
  $cid = !empty($item['cid']) ? $item['cid'] : ('new' . ++$new_cid_count);
  $name = !empty($item['name']) ? $item['name'] : '';
  $settings = $instance['widget']['settings'] + $instance['settings'] + $field['settings'];

  // Make sure this file gets loaded on submit
  $form_state['build_info']['files'][] = array('type' => 'inc', 'module' => 'availability_calendar', 'name' => 'availability_calendar.widget');


  $description = '';
  if ($settings['allow_disable']) {
    $description .= t("Uncheck the checkbox if you don't want a calendar at all for this entity. ");
  }
  $description .= t('To update the calendar: select a state and click on a begin and end date. Repeat if needed. When finished, click on <em>@button</em>.', array('@button' => t('Save')));
  $element = array_merge($element, array(
    '#type' => 'container',
    '#element_validate' => array('availability_calendar_field_widget_month_form_validate'),
    '#attached' => availability_calendar_field_widget_month_attach_js_css($cid, $name, $settings),
  ));
  $element['title_description'] = array(
    '#type' => 'item',
    '#title' => $element['#title'],
    '#description' => $description,
  );
  $element['enabled'] = array(
    '#type' => 'checkbox',
    // Don't show the checkbox if the user may not disable the calendar.
    '#access' => $settings['allow_disable'],
    '#title' => t('Enable an availability calendar for this @entity', array('@entity' => $element['#entity_type'])),
    '#default_value' => isset($item['enabled']) ? $item['enabled'] : 1,
    '#attributes' => array('class' => array('availability-enable')),
  );

  $element['calendar_details_div'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="availability-details">',
  );

  $element['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Useful to distinghuish calendars when you have multiple calendar values for this field.'),
    '#default_value' => isset($item['name']) ? $item['name'] : '',
  );
  $element['availability_states'] = array(
    '#type' => 'radios',
    '#title' => t('States'),
    '#default_value' => $settings['default_state'],
    '#options' => availability_calendar_get_states(array_filter($settings['allowed_states']), 'label'),
    '#attributes' => array('class' => array('availability-states')),
  );
  $element['availability_calendar'] = array(
    '#type' => 'markup',
    '#theme' => $instance['widget']['type'],
    '#markup' => '',
    '#cid' => $cid,
    '#name' => $name,
    '#mode' => 'all',
    '#settings' => $settings,
  );
  $element['calendar_details_enddiv'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );
  $element['availability_changes'] = array(
    '#type' => 'hidden',
    '#title' => t('Changes in availability'),
    '#default_value' => '',
    '#attributes' => array('class' => array('availability-changes')),
  );
  // Add element cid. It does not have to be sent to the client, but is used on
  // submit. Store 0 for new calendars.
  $element['cid'] = array(
    '#type' => 'hidden',
    '#access' => FALSE,
    '#default_value' => (int) $cid,
  );
  // Add the unique cid, used to match changes, fields, and elements in the
  // processing phase.
  $element['cid_unique'] = array(
    '#type' => 'hidden',
    '#access' => FALSE,
    '#default_value' => $cid,
  );
  return $element;
}

/**
 * Attaches the necessary javascript files and settings as well as the
 * necessary css files. It also assures that the base javascript is added.
 *
 * @param int|string $cid
 *   Existing cid (int) or temporary cid for new calendars (string).
 * @param string $name
 * @param array $settings
 */
function availability_calendar_field_widget_month_attach_js_css($cid, $name, $settings) {
  $split_day = isset($settings['show_split_day']) && $settings['show_split_day'];
  availability_calendar_add_base_js($cid, $name, $split_day, 'all');
  $cid_quoted = $cid == (string) (int) $cid ? $cid : "'$cid'";
  $result = array(
    'js' => array(
      drupal_get_path('module', 'availability_calendar') . '/availability_calendar.edit.js',
      "Drupal.behaviors.availabilityCalendarEdit$cid = {
         attach: function(context, settings) {
           Drupal.availabilityCalendar.editor$cid = new Drupal.availabilityCalendar.Edit($cid_quoted);
         }
       };" => array('type' => 'inline', 'scope' => 'footer'),
    ),
    'css' => array(drupal_get_path('module', 'availability_calendar') . '/availability_calendar.edit.css'),
  );
  return $result;
}

/**
 * Callback to validate the calendar changes.
 * @link http://api.drupal.org/api/drupal/developer--topics--forms_api_reference.html/7#element_validate
 */
function availability_calendar_field_widget_month_form_validate($element, &$form_state, $form) {
  $op = isset($form_state['clicked_button']) ? $form_state['clicked_button']['#id'] : '';
  if ($op == 'edit-submit') {
    $changes = array();
    $lines = explode("\n", $element['availability_changes']['#value']);
    // Remove the last incomplete line.
    array_pop($lines);
    foreach ($lines as $line) {
      if (!empty($line)) {
        $change = availability_calendar_field_widget_month_form_validate_line($line, $element);
        if ($change !== FALSE) {
          $changes[] = $change;
        }
        else {
          form_error($element, t('The requested calendar changes contain an invalid request.'));
          break;
        }
      }
    }
    // If the field changes validated, store them for processing in the submit
    // phase. In the submit phase our hook_field_attach_submit implementation
    // will process the changes. As that hook operates on the entity, we need
    // to be able to match these changes with the entity, field, language, and
    // delta. We do so using the value of the cid_unique field.
    if (!empty($changes) && $change !== FALSE) {
      $form_state['availability_calendar_updates'][] = array($element['#field_name'], $element['#language'], $element['#delta'], $element['cid_unique']['#value'], $changes);
    }
  }
}

/**
 * Validates a single command line.
 *
 * @param string $line
 *   Command line: "state: <sid> from: yyyy-mm-dd to: yyyy-mm-dd".
 * @return boolean|array
 *   An array with key 'from', 'to' and 'state' representing the parsed and
 *   validated command line or FALSE on validation errors.
 */
function availability_calendar_field_widget_month_form_validate_line($line, $element) {
  // Basic syntax checking.
  $parts = explode(' ', trim($line));
  if (count($parts) !== 6
   || $parts[0] !== 'state:'
   || $parts[2] !== 'from:' || preg_match('/^[1-2][0-9]{3}-[0-1][0-9]-[0-3][0-9]$/', $parts[3]) !== 1
   || $parts[4] !== 'to:' || preg_match('/^[1-2][0-9]{3}-[0-1][0-9]-[0-3][0-9]$/', $parts[5]) !== 1) {
    return FALSE;
  }
  // Check state: allowed state.
  $state = $parts[1];
  if (!array_key_exists($state, $element['availability_states']['#options'])) {
    return FALSE;
  }
  // Check dates: valid dates, in between ranges of the calendar and from <= to.
  $year = date('Y');
  $month = date('m');
  if (!checkdate(substr($parts[3], 5, 2), substr($parts[3], 8, 2), substr($parts[3], 0, 4))
    || $parts[3] < date(AC_ISODATE, mktime(0, 0, 0, $month, 1, $year))) {
    return FALSE;
  }
  $months = $element['availability_calendar']['#settings']['show_number_of_months'];
  if (!checkdate(substr($parts[5], 5, 2), substr($parts[5], 8, 2), substr($parts[5], 0, 4))
   || $parts[5] > date(AC_ISODATE, mktime(0, 0, 0, $month + $months + 1, 0, $year))) {
    return FALSE;
  }
  $from = new DateTime($parts[3]);
  $to = new DateTime($parts[5]);
  if ($from > $to) {
    return FALSE;
  }
  return array('state' => $state, 'from' => $from, 'to' => $to);
}

/**
 * Implements hook_field_attach_submit.
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_attach_submit/7
 */
function availability_calendar_field_attach_submit($entity_type, $entity, $form, &$form_state) {
  if (!empty($form_state['availability_calendar_updates'])) {
    foreach ($form_state['availability_calendar_updates'] as $update_info) {
      // A form can contain multiple entities with multiple calendar fields:
      // match the changes with specific fields using the cid_unique value.
      list($field_name, $language_code, $delta, $cid_unique, $changes) = $update_info;
      if (isset($entity->{$field_name}[$language_code][$delta]['cid_unique'])
        && $entity->{$field_name}[$language_code][$delta]['cid_unique'] === $cid_unique) {
        // If cid is not yet set (i.e. it is a new calendar), cid will get its
        // value from availability_calendar_update_multiple_availability(). That
        // is why the cid is a reference to the property of the entity object.
        $cid = &$entity->{$field_name}[$language_code][$delta]['cid'];
        $cid = availability_calendar_update_multiple_availability($cid, $changes);
      }
    }
  }
}
