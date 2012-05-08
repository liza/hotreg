<?php
/**
 * @file
 * Availability Calendar module. Defines an availability calendar field.
 *
 * Originally based on the Availability Calendars Module and the availability
 * module. This file contains the hooks and other functions that must be
 * present in the .module file.
 *
 * * Providing a field requires:
 * - Defining a field
 *   - hook_field_info()
 *   - hook_field_schema()
 *   - hook_field_validate()
 *   - hook_field_is_empty()
 *
 * - Defining a formatter for the field (the portion that outputs the field for
 *   display)
 *   - hook_field_formatter_info()
 *   - hook_field_formatter_view()
 *
 * - Defining a widget for the edit form
 *   - hook_field_widget_info()
 *   - hook_field_widget_form()
 *
 * See @link field_types Field Types API @endlink
 *
 * @author Erwin Derksen (http://drupal.org/user/750928)
 */

/***************************************************************
 * Field Type API hooks
 ***************************************************************/

/**
 * Implements hook_field_info().
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_info/7
 *
 * Provides the descriptions of the fields offered by this module.
 */
function availability_calendar_field_info() {
  // Get the states. They are used to determine some default settings.
  module_load_include('inc', 'availability_calendar', 'availability_calendar');
  $states = availability_calendar_get_states();
  if (!empty($states[1]) && $states[1]['css_class'] == 'cal-nc') {
    // State "Not communicated" exists, make this the default, but allow only
    // other states to be selected by the editor.
    $allowed_states = $states;
    unset($allowed_states[1]);
    $allowed_states = array_fill_keys(array_keys($allowed_states), 1);
    $default_state = 1;
  }
  else {
    $allowed_states = array(); // No state selected = all states.
    $default_state = 0; // Required, but no default.
  }
  return array(
    'availability_calendar' => array(
      'label' => t('Availability Calendar'),
      'description' => t('Allows to add an availability calendar to your entities.'),
      // States are not stored in the field settings but in their own table.
      // Fields can select a subset of this global set as allowed states.
      'settings' => array(
        'allowed_states' => $allowed_states,
        'default_state' => $default_state,
      ),
      'instance_settings' => array(), // No instance settings (so far).
      'default_widget' => 'availability_calendar',
      'default_formatter' => 'availability_calendar_viewport',
      'i18n_sync_callback' => 'availability_calendar_i18n_sync_availability_calendar_field',
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 * @link http://api.drupal.org/api/drupal/modules--field_ui--field_ui.api.php/function/hook_field_settings_form/7
 *
 * Availability Calendar field sets the allowed states and the default state at
 * the field level.
 */
function availability_calendar_field_settings_form($field, $instance, $has_data) {
  module_load_include('inc', 'availability_calendar', 'availability_calendar.widget');
  $settings = $field['settings'];
  $form = array();

  $form['allowed_states'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allowed states'),
    '#description' => t('Check the availability states that editors are allowed to select for this calendar instance.') . ' ' . t('Checking no states at all will allow all states.'),
    '#default_value' => $settings['allowed_states'],
    '#options' => availability_calendar_get_states('label'),
    '#weight' => 10,
  );
  $form['default_state'] = array(
    '#type' => 'select',
    '#title' => t('Default state'),
    '#default_value' => $settings['default_state'],
    '#required' => TRUE,
    '#options' => availability_calendar_get_states('label'),
    '#description' => t('Define the state for days that not yet have a state set. This does not have to be an allowed state.'),
    '#weight' => 11,
  );

  return $form;
}

/**
 * Implements hook_field_instance_settings_form().
 * @link http://api.drupal.org/api/drupal/modules--field_ui--field_ui.api.php/function/hook_field_instance_settings_form/7
 *
 * Availability Calendar field has no instance settings.
 */
function availability_calendar_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];
  $form = array();
  return $form;
}

/**
 * Implements hook_field_is_empty().
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_is_empty/7
 *
 * A calendar is never considered empty as we need to store the value of the
 * checkbox, even when it is disabled and no cid has been atached yet. This way,
 * we can reproduce the state of that checkbox.
 */
function availability_calendar_field_is_empty($item, $field) {
  return FALSE;
}

/**
 * Implements hook_field_validate().
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_validate/7
 *
 * Verifies that the calendar is valid. The calendar field itself is always
 * valid, though a specific widget form may fail validation. This is to be
 * handled by #element_validate's defined by the widget itself.
 */
function availability_calendar_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
}


/**************************************************************************
 * Field Type API: Widget
 *
 * The widget is the form element used to receive input from the user
 * when the field is being populated.
 **************************************************************************/

/**
 * Implements hook_field_widget_info.
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_widget_info/7
 *
 * This module defines a widget based on a monthly display, optionally in a
 * vieport. Other widgets may be defined in the future or by other modules.
 */
function availability_calendar_field_widget_info() {
  $settings = array(
    'allow_disable' => 1,
    'show_number_of_months' => 18,
    'first_day_of_week' => 1, // 1 = saturday = default change over day
    'show_week_number' => 0,
    'show_only_first_letter' => 0,
    'show_split_day' => 0,
  );
  $viewport_settings = $settings + array('viewport' => array(
    'cols' => 3,
    'rows' => 2,
    'scroll' => 1,
    'button_placement' => 'before',
  ));
  $field_types = array('availability_calendar');
  $behaviors = array(
    'multiple values' => FIELD_BEHAVIOR_DEFAULT,
    'default value' => FIELD_BEHAVIOR_NONE,
  );
  return array(
    'availability_calendar' => array(
      'label' => t('Shows the calendar as a number of months'),
      'description' => '',
      'field types' => $field_types,
      'settings' => $settings,
      'behaviors' => $behaviors,
    ),
    'availability_calendar_viewport' => array(
      'label' => t('Edits the calendar in a viewport'),
      'description' => t('Allows to define the number of months to show at once, and provides buttons to scroll through the months. Takes up less space.'),
      'field types' => $field_types,
      'settings' => $viewport_settings,
      'behaviors' => $behaviors,
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form
 * @link http://api.drupal.org/api/drupal/modules--field_ui--field_ui.api.php/function/hook_field_widget_settings_form/7
 */
function availability_calendar_field_widget_settings_form($field, $instance) {
  $form = array();
  $widget = $instance['widget'];
  $type = $widget['type'];
  $settings = $widget['settings'];

  $form['allow_disable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow to disable the calendar for a @type', array('@type' => $instance['bundle'])),
    '#default_value' => $settings['allow_disable'],
    '#weight' => 1,
  );
  $form += availability_calendar_field_widget_and_formatter_settings_form(TRUE, $type, $settings);

  return $form;
}

/**
 * Implements hook_field_widget_form().
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_widget_form/7
 *
 * The widget that is defined contains a checkbox to enable/disable the calendar,
 * displays a number of months and a set of radio buttons for the allowed states.
 *
 * Changing the calendar is done by selecting a state, and then defining a range
 * of states that should get the selected state.
 */
function availability_calendar_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  module_load_include('inc', 'availability_calendar', 'availability_calendar.widget');
  $element = availability_calendar_field_widget_month_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);

  return $element;
}


/******************************************************************************
 *  Field Type API: Formatter
 *
 *  These are the api hooks that present formatted (themed) output to the user.
 *****************************************************************************/

/**
 * Implements hook_field_formatter_info().
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_formatter_info/7
 *
 * This module defines a formatter based on a monthly display, optionally in a
 * vieport. Other formatters may be defined in the future or by other modules.
 */
function availability_calendar_field_formatter_info() {
  $settings = array(
    'show_number_of_months' => 12,
    'first_day_of_week' => variable_get('date_first_day', 1), // 1 = saturday = default change over day
    'show_week_number' => 0,
    'show_only_first_letter' => 0,
    'show_split_day' => 1,
    'selectable' => module_exists('availability_calendar_booking_formlet'),
  );
  $viewport_settings = $settings + array('viewport' => array(
    'cols' => 3,
    'rows' => 1,
    'scroll' => 1,
    'button_placement' => 'after',
  ));
  return array(
    'availability_calendar' => array(
      'label' => t('Show the calendar as a set of months'),
      'field types' => array('availability_calendar'),
      'settings' => $settings,
    ),
    'availability_calendar_viewport' => array(
      'label' => t('Show the calendar in a viewport'),
      'description' => t('Allows to define the number of months to show at once, and provides buttons to scroll through the months. Takes up less space.'),
      'field types' => array('availability_calendar'),
      'settings' => $viewport_settings,
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 * @link http://api.drupal.org/api/drupal/modules--field_ui--field_ui.api.php/function/hook_field_formatter_settings_form/7
 */
function availability_calendar_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $element = array();
  $display = $instance['display'][$view_mode];
  $type = $display['type'];
  $settings = $display['settings'];

  $element = availability_calendar_field_widget_and_formatter_settings_form(FALSE, $type, $settings);

  return $element;
}

/**
 * Returns the common elements for the widget and formatter settings forms.
 *
 * The widget and the formatter have almost all their settings in common. This
 * method returns the form elements that are commmon to both.
 *
 * @param boolean $widget
 *   Whether we are creating the form elements for the widget (true) or the
 *   formatter (false).
 * @param string $type
 *   The widget or formatter type. Both use the same names:
 *   availability_calendar or availability_calendar_viewport.
 * @param array $settings
 *   The settings to use for the default vaules.
 */
function availability_calendar_field_widget_and_formatter_settings_form($widget, $type, $settings) {
  $element = array();
  $element['show_number_of_months'] = array(
    '#type' => 'textfield',
    '#title' => t('Total number of months to @show', array('@show' => $widget ? t('edit') : t('show'))),
    '#default_value' => $settings['show_number_of_months'],
    '#required' => TRUE,
    '#element_validate' => array('_element_validate_integer_positive'),
  );
  if ($widget) {
    $element['show_number_of_months']['#description'] = t('The number of months to show when the calendar is being edited. Setting this larger than the number of months shown to normal visitors, allows editors to enter information into future calendar months before it is made publicly available.');
  }
  $element['first_day_of_week'] = array(
    '#type' => 'select',
    '#title' => t('First day of week'),
    '#default_value' => $settings['first_day_of_week'],
    // Use ISO-8601 week day numbers.
    '#options' => array(
      1 => t('Monday'),
      2 => t('Tuesday'),
      3 => t('Wednesday'),
      4 => t('Thursday'),
      5 => t('Friday'),
      6 => t('Saturday'),
      7 => t('Sunday'),
    ),
  );
  $element['show_week_number'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the week number before each row of the calendar'),
    '#default_value' => $settings['show_week_number'],
  );
  $element['show_only_first_letter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show only the first letter from the day of the week'),
    '#default_value' => $settings['show_only_first_letter'],
  );
  $element['show_split_day'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show split day states'),
    '#description' => t('Check this option if you want to show bookings from pm to am next morning.'),
    '#default_value' => $settings['show_split_day'],
  );
  if (!$widget) {
    $element['selectable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Interactive'),
      '#description' => t('Check this option if you want to let users interact with the calendar by clicking on available dates.'),
      '#default_value' => $settings['selectable'],
    );
  }
  if ($type == 'availability_calendar_viewport') {
    $element['viewport'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Viewport settings'),
    );
    $element['viewport']['cols'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of months to show horizontally'),
      '#default_value' => $settings['viewport']['cols'],
      '#required' => TRUE,
      '#element_validate' => array('_element_validate_integer_positive'),
    );
    $element['viewport']['rows'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of months to show vertically'),
      '#default_value' => $settings['viewport']['rows'],
      '#required' => TRUE,
      '#element_validate' => array('_element_validate_integer_positive'),
    );
    $element['viewport']['scroll'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of rows or cols to scroll'),
      '#description' => t('If the number of rows is 1, the viewport will scroll horizontally, otherwise it will scroll vertically.'),
      '#default_value' => $settings['viewport']['scroll'],
      '#required' => TRUE,
      '#element_validate' => array('_element_validate_integer_positive'),
    );
    $options = array(
      'before' => t('Before'),
      'after' => t('After'),
      'before_after' => t('1 Before and 1 after'),
      'not' => t('Not'),
    );
    $element['viewport']['button_placement'] = array(
      '#type' => 'select',
      '#title' => t('Button placement'),
      '#description' => t("Define where you want the have the buttons placed. If selecting '@not', you will have to define your own buttons.", array('@not' => t('Not'))),
      '#default_value' => $settings['viewport']['button_placement'],
      '#options' => $options,
      '#required' => TRUE,
    );
  }
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 * @link http://api.drupal.org/api/drupal/modules--field_ui--field_ui.api.php/function/hook_field_formatter_settings_summary/7
 */
function availability_calendar_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = array();

  $summary[] = format_plural($settings['show_number_of_months'], 'Show 1 month', 'Show @count months');
  if ($display['type'] == 'availability_calendar_viewport') {
    $summary[] = format_plural($settings['viewport']['rows'],
      'in a viewport of @horizontal by 1 month',
      'in a viewport of @horizontal by @count months',
      array('@horizontal' => $settings['viewport']['cols']));
  }

  return implode(' ', $summary);
}

/**
 * Implements hook_field_formatter_view().
 * @link http://api.drupal.org/api/drupal/modules--field--field.api.php/function/hook_field_formatter_view/7
 */
function availability_calendar_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  $settings = $display['settings'] + $instance['settings'] + $field['settings'];
  foreach ($items as $delta => $item) {
    if ($item['enabled'] == '1') {
      $element[$delta] = array(
        '#theme' => $display['type'],
        '#mode' => $settings['selectable'] ? 'available' : 'none',
        '#settings' => $settings,
        '#langcode' => $langcode,
        '#cid' => $item['cid'],
        '#name' => !empty($item['name']) ? $item['name'] : '',
      );
    }
  }

  return $element;
}
