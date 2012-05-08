<?php
/**
 * @file
 * Rules actions definitions for rules better message module
 *
 * @addtogroup rules
 * @{
 */

function rules_better_message_send_message($message, $type, $other, $clean, $repeat) {
  if (isset($other) && ($other != '')) {
    $type = $other;
  }
  rules_better_message_clean_messages($clean);
  drupal_set_message(filter_xss_admin($message), $type, $repeat);
}

function rules_better_message_clean_messages($clean, $type, $other) {
  if ($clean == 1) {
    drupal_get_messages();
  } 
  elseif ($clean == 2) {
    if (isset($other) && ($other != '')) {
      $type = $other;
    }
    drupal_get_messages($type);
  }
}
