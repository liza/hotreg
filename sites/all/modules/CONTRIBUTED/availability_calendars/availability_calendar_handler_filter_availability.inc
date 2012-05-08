<?php

/**
 * @class availability_calendar_handler_filter_availability Views handler to
 * filter on availability.
 *
 * This filter inherits from @see views_handler_filter as inheriting from
 * views_handler_filter_numeric or views_handler_filter_date did not turn out to
 * be a lot easier.
 *
 * This filter allows to filter on availability by accepting the following
 * values:
 * - 1 date (for availability at that given date)
 * - Arrival and departure date
 * - Arrival date and duration
 */
class availability_calendar_handler_filter_availability extends views_handler_filter {
  public static $instance;

  public function __construct() {
    self::$instance = $this;
    $this->always_multiple = TRUE;
  }

  public function option_definition() {
    $options = parent::option_definition();

    $options['value'] = array(
      'contains' => array(
        'arrival' => array('default' => ''),
        'departure' => array('default' => ''),
        'duration' => array('default' => ''),
      ),
    );
    $options['operator'] = array('default' => 'arrival_departure');

    return $options;
  }

  public function operators() {
    $operators = array(
      'at' => array(
        'title' => t('At (date)'),
        'method' => 'op_at',
        'summary' => t('at %arrival'),
        'values' => array('arrival'),
      ),
      'arrival_departure' => array(
        'title' => t('From arrival to departure'),
        'method' => 'op_arrival_departure',
        'summary' => t('From %arrival to %departure'),
        'values' => array('arrival', 'departure'),
      ),
      'arrival_duration' => array(
        'title' => t('From arrival during duration'),
        'method' => 'op_arrival_duration',
        'summary' => t('From %arrival during %duration days'),
        'values' => array('arrival', 'duration'),
      ),
    );

    return $operators;
  }

  /**
   * Provides a list of all the availability operators, optionally restricted
   * to only the given property of the operators.
   */
  public function operator_options($which = 'title') {
    $options = array();
    foreach ($this->operators() as $id => $operator) {
      $options[$id] = $operator[$which];
    }
    return $options;
  }

  public function operators_by_value($value) {
    $options = array();
    foreach ($this->operators() as $id => $operator) {
      if (in_array($value, $operator['values'])) {
        $options[] = $id;
      }
    }
    return $options;
  }

  /**
   * Add validation and date popup(s) to the value form.
   */
  public function value_form(&$form, &$form_state) {
    $form['value']['#tree'] = TRUE;

    if (empty($form_state['exposed'])) {
      // We're in Views edit mode self. Add validator here. When we're in an
      // exposed form, validation will go via exposed_validate().
      $form['value']['#element_validate'][] = 'availability_calendar_handler_filter_availability_validate_value';
    }

    // Determine values to add and their dependencies.
    $dependency_source = null;
    if (!empty($form_state['exposed']) && (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator_id']))) {
      // Exposed form with operator not exposed: only add values for the
      // set operator.
      $operators = $this->operators();
      $values = $operators[$this->operator]['values'];
    }
    else {
      // Views UI
      $values = array('arrival', 'departure', 'duration');
        if (!empty($form['operator'])) {
          $dependency_source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
        }
    }

    // Add value fields.
    if (in_array('arrival', $values)) {
      $form['value']['arrival'] = array(
        '#type' => 'textfield',
        '#title' => $this->operator == 'at' ? t('At') : t('Arrival date'),  //@todo
        '#size' => 12,
        '#default_value' => $this->value['arrival'],
      );
      if (module_exists('date_popup')) {
        $this->change_element_into_date_popup($form['value']['arrival']);
      }
      if ($dependency_source !== null) {
        $form['value']['arrival']['#dependency'] = array($dependency_source => $this->operators_by_value('arrival'));
      }
    }
    if (in_array('departure', $values)) {
      $form['value']['departure'] = array(
        '#type' => 'textfield',
        '#title' => t('Departure date'),
        '#size' => 12,
        '#default_value' => $this->value['departure'],
      	//@todo'#dependency' => array($source => $this->operator_values(2)),
      );
      if (module_exists('date_popup')) {
        $this->change_element_into_date_popup($form['value']['departure']);
      }
      if ($dependency_source !== null) {
        $form['value']['departure']['#dependency'] = array($dependency_source => $this->operators_by_value('departure'));
      }
    }
    if (in_array('duration', $values)) {
      $night = t('night');
      $nights = t('nights');
      $week = t('week');
      $weeks = t('weeks');
      $month = t('month');
      $options = array(
         0 => t('- Select duration -'),
         7 => "1 $week",
         1 => "1 $night",
         2 => "2 $nights",
         3 => "3 $nights",
         4 => "4 $nights",
         5 => "5 $nights",
         6 => "6 $nights",
         8 => "8 $nights",
         9 => "9 $nights",
        10 => "10 $nights",
        11 => "11 $nights",
        12 => "12 $nights",
        14 => "2 $weeks",
        21 => "3 $weeks",
        28 => "4 $weeks",
        31 => "1 $month",
      );
      $form['value']['duration'] = array(
        '#type' => 'select',
        '#title' => t('Duration'),
    		'#options' => $options,
      	'#default_value' => $this->value['duration'],
      );
      if ($dependency_source !== null) {
        $form['value']['duration']['#dependency'] = array($dependency_source => $this->operators_by_value('duration'));
      }
    }
  }

  /**
   * Changes a (text) form element into a dete popup element.
   *
   * @param array $element
   */
  protected function change_element_into_date_popup(&$element) {
    $element['#type'] = 'date_popup';
    $element['#date_label_position'] = '';
    $element['#date_type'] = 'DATE_ISO';
    $element['#date_format'] = 'Y-m-d';
    $element['#date_year_range'] = '-0:+2';
  }

  /**
   * Validates our part of the exposed form.
   *
   * Overrides @see views_handler::exposed_validate().
   */
  public function exposed_validate(&$form, &$form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }

    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator_id'])) {
      // Operator is exposed as well: get it from the form state.
      $operator = $form_state['values'][$this->options['expose']['operator_id']];
    }
    else {
      $operator = $this->operator;
    }

    $this->validate_value($form[$this->options['expose']['identifier']], $form_state);
  }

  /**
   * Validate that the values convert to something usable.
   */
  public function validate_value(&$element, $form_state) {
    // In Views UI, the value is required if the filter is not exposed.
    // In exposed form, values are required if "Required" was checked.
    if (empty($form_state['exposed'])) {
      $required = !$form_state['values']['options']['expose_button']['checkbox']['checkbox'];
      $operator = $form_state['values']['options']['operator'];
    }
    else {
      $required = (bool) $this->options['expose']['required'];
      $operator = empty($this->options['expose']['use_operator']) ? $this->operator : $form_state['values'][$this->options['expose']['operator_id']];
    }
    $operators = $this->operators();
    $values = empty($operator) ? array('arrival', 'departure', 'duration') : $operators[$operator]['values'];

    $yesterday = new DateTime();
    // PHP5.3 $yesterday = $yesterday->modify('-1 day')->format(AC_ISODATE);
    $yesterday->modify('-1 day');
    $yesterday = $yesterday->format(AC_ISODATE);

    $value = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
    $arrival_valid = FALSE;
    if (in_array('arrival', $values) && array_key_exists('arrival', $value)) {
      $arrival_valid = $this->validate_valid_time_1($element['arrival'], $value['arrival'], $required, $yesterday, t('Only future availability can be searched.'));
    }
    if (in_array('departure', $values) && array_key_exists('departure', $value)) {
      $this->validate_valid_time_1($element['departure'], $value['departure'], $required || !empty($value['arrival']), $arrival_valid ? $value['arrival'] : NULL, t('The departure date should be after the arrival date.'));
    }
    if (in_array('duration', $values) && array_key_exists('duration', $value)) {
      $this->validate_valid_duration($element['duration'], $value['duration'], $required || !empty($value['arrival']));
    }
  }

  protected function validate_valid_time_1(&$element, $value, $required, $minimum, $minimum_error_message) {
    $valid = TRUE;
    // Note that the value can be an array with a date and time component.
    if (is_array($value)) {
      $value = $value['date'];
    }
    if (empty($value)) {
      if ($required) {
        form_error($element, t('Field %field is required.', array('%field' => $element['#title'])));
        $valid = FALSE;
      }
    }
    else if (preg_match('/^[1-2][0-9]{3}-[0-1][0-9]-[0-3][0-9]$/', $value) !== 1 || !checkdate(substr($value, 5, 2), substr($value, 8, 2), substr($value, 0, 4))) {
      form_error($element, t('Invalid date format.'));
      $valid = FALSE;
    }
    else if (!empty($minimum) && $value <= $minimum) {
      form_error($element, $minimum_error_message);
      $valid = FALSE;
    }
    return $valid;
  }

  protected function validate_valid_duration(&$element, $value, $required) {
    $valid = TRUE;
    if (empty($value)) {
      if ($required) {
        form_error($element, t('Field %field is required.', array('%field' => $element['#title'])));
        $valid = FALSE;
      }
    }
    else if ((!is_int($value) && !ctype_digit($value)) || $value <= 0) {
      form_error($element, t('Duration must be a positive number.'));
      $valid = FALSE;
    }
    return $valid;
  }

  /**
  * Check to see if input from the exposed filters should change
  * the behavior of this filter.
  */
  public function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator_id']) && isset($input[$this->options['expose']['operator_id']])) {
      // Fetch operator from form (instead of from $this object)
      $this->operator = $input[$this->options['expose']['operator_id']];
    }

    if (!empty($this->options['expose']['identifier'])) {
      // Fetch value from form (instead of from $this object)
      $this->value = $input[$this->options['expose']['identifier']];

      // Check if the values are filled in, if not, we don't want to change the
      // query.
      $operators = $this->operators();
      $values = $operators[$this->operator]['values'];
      foreach ($values as $value_name) {
        if (empty($this->value[$value_name])) {
          return FALSE;
        }
      }
    }

    return TRUE;
  }

  public function query() {
    $this->ensure_my_table();
    $operators = $this->operators();
    if (isset($operators[$this->operator]['method'])) {
      $this->{$operators[$this->operator]['method']}();
    }
  }

  protected function op_at() {
    $this->value['duration'] = 1;
    return $this->op_arrival_duration();
  }

  protected function op_arrival_departure() {
    module_load_include('inc', 'availability_calendar', 'availability_calendar.widget');
    availability_calendar_query_available($this->query, $this->table_alias, $this->real_field, new DateTime($this->value['arrival']), new DateTime($this->value['departure']), $this->definition['default_state']);
  }

  protected function op_arrival_duration() {
    module_load_include('inc', 'availability_calendar', 'availability_calendar.widget');
    availability_calendar_query_available($this->query, $this->table_alias, $this->real_field, new DateTime($this->value['arrival']), $this->value['duration'], $this->definition['default_state']);
  }

  public function admin_summary() {
    $output = '';
    if (!empty($this->options['exposed'])) {
      $output = t('exposed');
    }
    else {
      $operators = $this->operators();
      if (isset($operators[$this->operator]['summary'])) {
        $arguments = array();
        foreach($this->value as $key => $value) {
          $arguments["@$key"] = $value;
        }
        $output = format_string($operators[$this->operator]['summary'], $arguments);
      }
    }
    return $output;
  }
}

/**
 * Form element validator for the date field(s): forward to the
 * @see availability_calendar_handler_filter_availability::validate_value()
 * method inside the class.
 */
function availability_calendar_handler_filter_availability_validate_value(&$element, &$form_state, $form) {
  availability_calendar_handler_filter_availability::$instance->validate_value($element, $form_state);
}

/**
 * Called by hook_alter_js().
 *
 * This function changes the date popups added by the class above.
 */
function availability_calendar_handler_filter_availability_js_alter(&$javascript) {
  static $adapt = FALSE;
  if ($javascript === TRUE) {
    $adapt = TRUE;
    return;
  }
  if ($adapt && isset($javascript['settings']['data'])) {
    foreach ($javascript['settings']['data'] as &$setting) {
      if (is_array($setting) && isset($setting['datePopup'])) {
        foreach ($setting['datePopup'] as &$date_popup_settings) {
          $date_popup_settings['settings']['minDate'] = 0;
        }
      }
    }
  }
}
