<?php
// $Id$

/**
 * @file
 * Maximenu Admin Settings
 *
 * This is a way to enable/disable settings which may 
 * not be needed by the site and just clutters the configurations
 * of menus.
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Admin Form: UI
 *
 */
function maximenu_settings($form, &$form_state, $op = NULL) {
  
  $active_tab = variable_get('maximenu_active_tab', '');
  
  $form = array();
  $settings_form = _maximenu_settings();
  // List/Add/Edit/Delete
  $form['maximenu_settings'] = array(
    //'#type'  => 'fieldset',
    '#type'  => 'vertical_tabs',
    '#default_tab' => $active_tab,
    '#tree'   => TRUE,
  );    

  $form['maximenu_settings'] += $settings_form;
  
  $form['save'] = array( '#type' => 'submit', '#value' => t('Save'), );

  $form['cancel'] = array( '#type' => 'submit', '#value' => t('Cancel'), ); 

  //dsm($form);
  return $form;    

}


/**
 * Admin Form: Logic for getting the forms
 *
 */
function _maximenu_settings() {
  global $_maximenu_settings;
  $settings = $_maximenu_settings;
  
  // Gets the settings default options
  $plugins = parse_ini_file(MAXIMENU_PLUGINS . '/plugins.defaults', TRUE);
  //dsm($plugins);
  $form = array();

  foreach ($plugins as $plugin => $default) {
    // this will read available option files
    $plugin_options = _maximenu_options_get($plugin);
    $form[$plugin] = array(
      '#type'        => 'fieldset',
      '#title'       => t(strtoupper($plugin)),
      '#collapsible' => TRUE,
      '#collapsed'   => FALSE,
      '#description' => t('Options'),
    );    
    $form[$plugin]['options'] = array(
      '#type' => 'checkboxes',
      '#title' => t(''),
      '#options' => drupal_map_assoc($plugin_options),
      '#default_value' => isset($settings[$plugin]) ? $settings[$plugin]: $default,
    );    
  }
  return $form;
}


/**
 * Admin Form: Submit
 *
 */
function maximenu_settings_submit($form, $form_state) {
  $settings_all = '';
  $form_values = $form_state['values'];
  //dsm($form_values);
  if ($form_values['op'] == 'Save') {
    $settings_new = array();    
    $settings = $form_values['maximenu_settings'];
    foreach ($settings as $setting => $options) {
      //dsm($options['options']);
      // removes active tab from the array
      if ($setting != 'maximenu_settings__active_tab') {
        foreach ($options['options'] as $option => $value) {
          if ($value != '0') {
            $option = strtolower($option);
            $settings_new[$setting][$option] = $value;
          }
        }
        // add all setting names
        $settings_all .= $setting . ', ';
      }
      elseif ($setting == 'maximenu_settings__active_tab') {
        // this is saved in a variable because it seems that
        // $form_state array is not being picked up by the current form
        // to be used by the $default_tab var
        variable_set('maximenu_active_tab', $options);
      }  
    }
    drupal_set_message(t('Settings (%settings_all... ) have been saved.', array('%settings_all' => check_plain(ucwords($settings_all)))));
    variable_set('maximenu_settings', $settings_new);
    // it's necessary to rebuild theme for the changes in hook_theme() to take effect
    drupal_theme_rebuild();    
  }
  elseif ($form_values['op'] == 'Cancel') {
    drupal_goto('admin/structure/maximenu');
  }
}



