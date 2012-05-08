<?php
/**
 * @file
 * General helper methods for Availability Calendar
 * - database access
 *
 * @author Erwin Derksen (http://drupal.org/user/750928)
 */

/**
 * Adds the necessary base javascript files, settings and initialization for the
 * given calendar.
 *
 * @param int|string $cid
 *   Existing cid (int) or temporary cid for new calendars (string).
 * @param string $name
 * @param boolean $split_day
 * @param string $mode
 */
function availability_calendar_add_base_js($cid, $name, $split_day, $mode) {
  static $added = array();
  if (!isset($added[$cid])) {
    if (count($added) === 0) {
      // Add the base client side API.
      drupal_add_js(drupal_get_path('module', 'availability_calendar') .'/availability_calendar.js',
        array('group' => JS_LIBRARY));

      // Initialize the states
      drupal_add_js(array('availabilityCalendar' => array('states' => availability_calendar_get_states('bool'))),
        array('type' => 'setting', 'group' => JS_LIBRARY));
      drupal_add_js("Drupal.behaviors.availabilityCalendar = {
          attach: function(context, settings) {
            Drupal.availabilityCalendar.states = new Drupal.availabilityCalendar.States(settings.availabilityCalendar.states);
          }
        };",
        array('type' => 'inline', 'group' => JS_LIBRARY, 'scope' => 'footer'));
    }
    $split_day = $split_day ? 'true' : 'false';
    $cid_quoted = $cid == (string) (int) $cid ? $cid : "'$cid'";
    drupal_add_js("Drupal.behaviors.availabilityCalendar$cid = {
        attach: function(context, settings) {
          Drupal.availabilityCalendar.get($cid_quoted, '$name', $split_day, '$mode');
        }
      };",
      array('type' => 'inline', 'group' => JS_LIBRARY, 'scope' => 'footer'));
    $added[$cid] = TRUE;
  }
}


/*
 * DATABASE ACCESS FUNCTIONS
 * -------------------------
 */

/**
 * Returns an array of records (or labels) of states keyed by sid.
 *
 * The results can be processed or filtered based on arguments passed in:
 * - bool: filter on is_available
 * - string: process array based on the value of the string:
 *     'label': only return label, not a record array
 *     'bool': convert is_available item to a real bool
 * - array: filter on sid (array keys should be a list of the allowed sid's)
 * Multiple filters/processors can be passed.
 *
 * @param boolean|string ...
 *   Processing or filtering to be done on the list of states.
 * @return array
 *   Array of records keyed by the sid.
 */
function availability_calendar_get_states() {
  $states = &drupal_static(__FUNCTION__);
  if ($states === NULL) {
    $states = db_select('availability_calendar_state')
      ->fields('availability_calendar_state')
      ->orderBy('weight')
      ->execute()
      ->fetchAllAssoc('sid', PDO::FETCH_ASSOC);
  }
  $result = $states;
  foreach (func_get_args() as $arg) {
    if (is_bool($arg)) {
      // filter out non-available states
      $result = array_filter($result, $arg ? 'availability_calendar_filter_available' : 'availability_calendar_filter_non_available');
    }
    else if (is_array($arg)) {
      // Filter out non-allowed states. If no states are passed in, all states
      // are allowed.
      if (!empty($arg)) {
        $result = array_intersect_key($result, $arg);
      }
    }
    else {
      array_walk($result, 'availability_calendar_convert_state', $arg);
    }
  }
  return $result;
}

/**
 * @see array_filter() callback to filter states based on whether they are
 * to be seen as available.
 *
 * @param array $state
 *   Array containing a state.
 */
function availability_calendar_filter_available($state) {
  return (bool) $state['is_available'];
}

/**
 * @see array_filter() callback to filter states based on whether they are
 * to be seen as not available.
 *
 * @param array $state
 *   Array containing a state.
 */
function availability_calendar_filter_non_available($state) {
  return !(bool) $state['is_available'];
}

/**
 * @see array_walk() callback to convert all states retrieved from the database
 * or to be stored in the database.
 * - Convert is_available from int to boolean.
 *
 * @param array $state
 *   Array containing a state.
 */
function availability_calendar_convert_state(&$state, $key, $op) {
  if ($op === 'bool') {
    $state['is_available'] = $state['is_available'] == 1;
  }
  else if ($op === 'label') {
    $state = t($state['label']);
  }
}

/**
 * Updates the set of states.
 *
 * @param array $states
 *   Array with the new state records (sid, css_class, label, weight,
 *   and is_available values).
 */
function availability_calendar_update_states($states) {
  $table_name = 'availability_calendar_state';
  $existing_states = availability_calendar_get_states();
  foreach ($existing_states as $sid => $existing_state) {
    $delete = TRUE;
    foreach ($states as $state) {
      if (isset($state['sid']) && $state['sid'] == $sid) {
        $delete = FALSE;
        break;
      }
    }
    if ($delete) {
      // Cascading delete: delete availability referring to this sid.
      db_delete('availability_calendar_availability')
        ->condition('sid', $sid)
        ->execute();
      // Delete state itself.
      db_delete($table_name)
        ->condition('sid', $sid)
        ->execute();
      // @todo: update field instances (warning if something changes?)
      // this omission leads to a: Warning: Illegal offset type in form_type_checkboxes_value() (line 2229 of includes\form.inc).
      unset($existing_states[$sid]);
    }
  }
  foreach ($states as $state) {
    $sid = isset($state['sid']) ? $state['sid'] : NULL;
    unset($state['sid']);
    if (!empty($sid) && array_key_exists($sid, $existing_states)) {
      // Update
      db_update($table_name)
        ->fields($state)
        ->condition('sid', $sid, '=')
        ->execute();
    }
    else {
      // Insert, sid will be created.
      db_insert($table_name)
        ->fields($state)
        ->execute();
    }
  }
  drupal_static_reset('availability_calendar_get_states');
}

/**
 * Creates a new calendar an returns its id (the "cid").
 *
 * @return int
 *   The cid of the newly created calendar.
 */
function availability_calendar_create_calendar() {
  $now = time();
  $cid = db_insert('availability_calendar_calendar')
    ->fields(array('created' => $now, 'changed' => $now))
    ->execute();
  return (int) $cid;
}

/**
 * Updates the changed field of a calendar.
 *
 * @param int $cid
 *   The calendar id.
 */
function availability_calendar_update_calendar($cid) {
  $now = time();
  $cid = db_update('availability_calendar_calendar')
    ->fields(array('changed' => $now))
    ->condition('cid', $cid, '=')
    ->execute();
  return (int) $cid;
}

/**
 * Gets a calendar.
 *
 * @param int $cid
 *   The id of the calendar to get.
 * @return NULL|array
 *   Calendar record.
 */
function availability_calendar_get_calendar($cid) {
  $calendar = db_select('availability_calendar_calendar')
    ->condition('cid', $cid, '=')
    ->execute()
    ->fetchAssoc();
  return $calendar;
}

/**
 * Returns the availability for the given calendar and date range.
 *
 * The from and to dates are inclusive.
 *
 * @param int $cid
 *   cid may be 0 for not yet existing calendars.
 * @param DateTime $from
 * @param DateTime $to
 * @param int|NULL $default_state
 *   If $default_state is NULL only the stored availability is returned,
 *   otherwise the returned array is completed with this default state for
 *   missing dates.
 * @return array
 *   Array with availability within the given date range indexed by date.
 */
function availability_calendar_get_availability($cid, $from, $to, $default_state = NULL) {
  // Get the states from the database.
  $availability = array();
  if (!empty($cid)) {
    $availability = db_select('availability_calendar_availability')
      ->fields('availability_calendar_availability', array('date', 'sid'))
      ->condition('cid', $cid)
      ->condition('date', array($from->format(AC_ISODATE), $to->format(AC_ISODATE)), 'BETWEEN')
      ->execute()
      ->fetchAllKeyed();
  }
  if (!empty($default_state)) {
    for ($date = clone $from; $date <= $to; $date->modify('+1 day')) {
      $day = $date->format(AC_ISODATE);
      if (!array_key_exists($day, $availability)) {
        $availability[$day] = $default_state;
      }
    }
  }
  return $availability;
}

/**
 * Updates/inserts the state for the given calendar for the given date range.
 *
 * Note that:
 * - $from and $to are both inclusive.
 * - $from and $to must be ordered.
 *
 * @param int $cid
 * @param int $sid
 * @param DateTime $from
 * @param Datetime $to
 * @param boolean $update_changed
 */
function availability_calendar_update_availability($cid, $sid, $from, $to, $update_changed = TRUE) {
  if ($update_changed) {
    availability_calendar_update_calendar($cid);
  }

  // Update the already existing dates.
  $count = db_update('availability_calendar_availability')
    ->fields(array('sid' => $sid))
    ->condition('cid', $cid, '=')
    ->condition('date', array($from->format(AC_ISODATE), $to->format(AC_ISODATE)), 'BETWEEN')
    ->execute();

  // Insert the non-existing dates.
  //PHP5.3: $days = $from->diff($to)->days + 1;
  $timestamp_from = (int) $from->format('U');
  $timestamp_to = (int) $to->format('U');
  $days = (int) round(($timestamp_to - $timestamp_from)/(60*60*24)) + 1;
  if ($count != $days) {
    // Get existing dates to know which ones to insert.
    $existing_availability = availability_calendar_get_availability($cid, $from, $to);

    $values = array('cid' => $cid, 'date' => null, 'sid' => $sid);
    $insert = db_insert('availability_calendar_availability')
      ->fields(array_keys($values));
    for ($day = clone $from; $day <= $to; $day->modify('+1 day')) {
      $values['date'] = $day->format(AC_ISODATE);
      if (!array_key_exists($values['date'], $existing_availability)) {
        $insert->values($values);
      }
    }
    $insert->execute();
  }
}

/**
 * Updates/inserts the states for the given ranges.
 *
 * @param NULL|int $cid
 *   The calendar id. If NULL, a new calendar will be created.
 * @param array $changes
 *   An array of changes each entry is an array with keys state, from and to.
 * @return int
 *   The cid (of the existing or newly created calendar).
 */
function availability_calendar_update_multiple_availability($cid, $changes) {
  if (empty($cid)) {
    $cid = availability_calendar_create_calendar();
  }
  else {
    availability_calendar_update_calendar($cid);
  }
  foreach ($changes as $change) {
    availability_calendar_update_availability($cid, $change['state'], $change['from'], $change['to'], FALSE);
  }
  return $cid;
}

/**
 * Adds a where clauses to the given query to filter on availability.
 *
 * Notes:
 * - The query will 'fail' when no calendar has been created yet and the default
 *   state is an "available" state. However I'm not sure whether this is a bug
 *   or correct behavior.
 * - If a field has multiple values, an entity may be returned multile times.
 *   However this function cannot filter on duplicates as it does not know
 *   whether just calendars should be returned, or fields or entities, and
 *   whether duplicates are expected or not.
 *
 * @param SelectQuery|views_plugin_query $query
 *   The query to add the clause to. This may be a default Drupal or a Views
 *   specific query object.
 * @param string $field_table
 *   The name of the (field data) table to join on.
 * @param string $cid_field
 *   The name of the field in the table (to join on) that contains the cid.
 * @param DateTime $arrival
 *   The date to start searching for availability.
 * @param int|DateTime $departure_or_duration
 *   Either the departure day (DateTime) or the duration of the stay (int).
 * @param int $default_state
 *   The default state to use for dates without availability assigned.
 */
function availability_calendar_query_available($query, $field_table, $cid_field, $arrival, $departure_or_duration, $default_state) {
  if (is_object($departure_or_duration) && get_class($departure_or_duration) === 'DateTime') {
    $departure = $departure_or_duration;
    //PHP5.3: $duration = $arrival->diff($departure)->days + 1;
    $timestamp_arrival = (int) $arrival->format('U');
    $timestamp_departure = (int) $departure->format('U');
    $duration = (int) round(($timestamp_departure - $timestamp_arrival)/(60*60*24));
  }
  else {
    $duration = (int) $departure_or_duration;
    $departure = clone $arrival;
    $departure->modify("+$duration days");
  }
  $states = availability_calendar_get_states();
  $default_state = $states[$default_state];
  $sids = array_keys(availability_calendar_get_states($default_state['is_available'] != 1));
  $to = clone $departure;
  $to->modify('-1 day');
  $subquery = db_select('availability_calendar_availability', 'availability_calendar_availability')
    ->where("availability_calendar_availability.cid = $field_table.$cid_field")
    ->condition('availability_calendar_availability.date', array($arrival->format(AC_ISODATE), $to->format(AC_ISODATE)), 'BETWEEN')
    ->condition('availability_calendar_availability.sid', $sids, 'IN');
  if ($default_state['is_available'] == 1) {
    // Default status = available: so no single day may be marked as non
    // available. Check by doing a check on the existence of a non available day
    // in the given period.
    $subquery->addExpression('1');
    if (is_a($query, 'views_plugin_query')) {
      $query->add_where(0, '', $subquery, 'NOT EXISTS');
    }
    else {
      $query->notexists($subquery);
    }
  }
  else {
    // Default status = not available: so all days must be marked as available.
    // Check by doing a count on the available days in the given period which
    // should equal the total number of days.
    //PHP5.3: $days = $arrival->diff($departure)->days;
    $subquery->addExpression('count(*)');
    if (is_a($query, 'views_plugin_query')) {
      $query->add_where(0, $duration, $subquery, 'IN');
    }
    else {
      $query->condition($duration, $subquery, 'IN');
    }
  }
}
