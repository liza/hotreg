<?php
/**
 * @file
 * Declare and handle FontFolio specific theme settings
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @TODO
 *   Add validation to Css color value
 */
function fontfolio_form_system_theme_settings_alter(&$form, $form_state) {

  // Fonfolio theme settings section (fieldset).
  $form['fonfolio'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Fonfolio theme settings'),
  );

  // Body background color.
  $form['fonfolio']['body_bg_color'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Background Color'),
    '#default_value' => theme_get_setting('body_bg_color'),
    '#description'   => t('Optionaly: set CSS background-color for whole page. Use valid css colors'),
  );

  $form['fonfolio']['hide_page_tile'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Hide page title on Taxonomy term pages'),
    '#default_value' => theme_get_setting('hide_page_tile'),
    '#description'   => t("By default, In fonfolio theme, we don't display the page title on taxonomy term pages. If you want to display The title uncheck this checkbox."),
  );

  // Social Urls.
  $form['fonfolio']['social'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Social Media URLs'),
    '#collapsible' => TRUE,
  );

  $form['fonfolio']['social']['facebook'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Facebook Link'),
    '#default_value'    => theme_get_setting('facebook'),
    '#description'      => t('Your Facebook page link'),
    '#element_validate' => array('fontfolio_social_url_validate'),
  );

  $form['fonfolio']['social']['twitter'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Twitter Link'),
    '#default_value' => theme_get_setting('twitter'),
    '#description'   => t('Your Twitter page link'),
    '#element_validate' => array('fontfolio_social_url_validate'),
  );

  $form['fonfolio']['social']['dribble'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Dribble Link'),
    '#default_value' => theme_get_setting('dribble'),
    '#description'   => t('Your Dribble page link'),
    '#element_validate' => array('fontfolio_social_url_validate'),
  );

  $form['fonfolio']['social']['plus'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Google+ Link'),
    '#default_value' => theme_get_setting('plus'),
    '#description'   => t('Your Google+ page link'),
    '#element_validate' => array('fontfolio_social_url_validate'),
  );
}

/**
 * Validate social urls with valid_url();
 */
function fontfolio_social_url_validate($element, &$form_state, $form) {
  if ($element['#value'] && !valid_url($element['#value'], TRUE)) {
    form_set_error($element['#name'], t('The @social_url is not valid URL. Please try to copy/paste it one more time.', array('@social_url' => $element['#title'])));
  }
}
