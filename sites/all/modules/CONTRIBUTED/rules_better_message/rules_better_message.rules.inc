<?php

/**
 * @file
 * Rules actions definitions for rules better message module
 */

/**
 * Implements hook_rules_file_info()
 */
function rules_better_message_rules_file_info() {
  return array('rules_better_message.eval');
}

/**
 * Implementation of hook_rules_action_info().
 */
function rules_better_message_rules_action_info() {
  $actions = array(
    'rules_better_message_send_message' => array(
      'label' => t('Send a message'),
      'group' => t('Messages'),
      'parameter' => array(
        'message' => array(
          'label' => t('Message'),
          'type' => 'text',
          'sanitize' => TRUE,
          'translatable' => TRUE,
        ),
        'type' => array(
          'type' => 'token',
          'label' => t('Message type'),
          'options list' => 'rules_better_message_types',
          'default value' => 'status',
          'optional' => TRUE,
        ),
        'other' => array(
          'label' => t('Other type'),
          'type' => 'text',
          'sanitize' => TRUE,
          'translatable' => TRUE,
          'optional' => TRUE,
        ),
        'clean' => array(
          'type' => 'token',
          'label' => t('Clean messages'),
          'description' => t("Set if and how to clean existing messages."),
          'default value' => 0,
          'options list' => 'rules_better_message_clean_options',
          'optional' => TRUE,
          'restriction' => 'input',
        ),
        'repeat' => array(
          'type' => 'boolean',
          'label' => t('Repeat message'),
          'description' => t("If disabled and the message has been already shown, then the message won't be repeated."),
          'default value' => TRUE,
          'optional' => TRUE,
          'restriction' => 'input',
        ),
      ),
    ),
    'rules_better_message_clean_messages' => array(
      'label' => t('Clean messages'),
      'group' => t('Messages'),
      'parameter' => array(
        'clean' => array(
          'type' => 'token',
          'label' => t('Clean messages'),
          'description' => t("Set if and how to clean existing messages."),
          'default value' => 0,
          'options list' => 'rules_better_message_clean_options',
          'optional' => TRUE,
          'restriction' => 'input',
        ),
        'type' => array(
          'type' => 'token',
          'label' => t('Message type'),
          'options list' => 'rules_better_message_types',
          'default value' => 'status',
          'optional' => TRUE,
        ),
        'other' => array(
          'label' => t('Other type'),
          'type' => 'text',
          'sanitize' => TRUE,
          'translatable' => TRUE,
          'optional' => TRUE,
        ),
      )
    ),
  );

  return $actions;
}

/**
 * Options list callback defining drupal_message types.
 */
function rules_better_message_types() {
  return array(
    'status' => t('Status'),
    'warning' => t('Warning'),
    'error' => t('Error'),
    'other' => t('Other type'),
  );
}

/**
 * Options list callback defining drupal_message cleanning options.
 */
function rules_better_message_clean_options() {
  return array(
    0 => t('Do not clean'),
    1 => t('Clean all'),
    2 => t('Clean selected type'),
  );
}