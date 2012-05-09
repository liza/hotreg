<?php
/* $Id$ */

/**
 * @file
 * Maximenu Style - Accordion
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Loads options to Maximenu admin
 *
 */ 
function _maximenu_accordion_options($menu_content = array(), $settings = array()) {

	$form_new = array();
 
	// Skin
  //$skin_options  = isset($settings['skins']) ? $settings['skins']: '';
	
	// Style - Accordion
  $form_new['autostart'] = array(
    '#type' => 'select',
    '#title' => t('Autostart'),
    '#options' => array('true' => t('True'), 'false' => t('False')),
		'#default_value' => isset($menu_content['autostart']) ? $menu_content['autostart']: 'true',
  );
  $form_new['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
		'#default_value' => isset($menu_content['speed']) ? $menu_content['speed']: 2000,
  );
  $form_new['slidenum'] = array(
    '#type' => 'select',
    '#title' => t('Show Slide Number'),
    '#options' => array('true' => t('True'), 'false' => t('False')),
		'#default_value' => isset($menu_content['slidenum']) ? $menu_content['slidenum']: 'false',
  );	
	
	// Effects
  //$effects_options  = isset($settings['effects']) ? $settings['effects']: '';
	
	// Others
  $others_options  = isset($settings['others']) ? $settings['others']: '';
	if (!empty($others_options)) {
  	if (!empty($others_options['active'])) {
      $form_new['active'] = array(
        '#type'  => 'checkbox',
        '#title' => t('Add "active-trail" to menu li when submenus are active'),
        '#default_value' => isset($menu_content['active']) ? $menu_content['active']: 0,
        '#description' => t('This will only be applied to menus with attached menu blocks'),       
      ); 
		}
    if (!empty($others_options['autoscroll'])) {
      $form_new['autoscroll'] = array(
       '#type'  => 'checkbox',
       '#title' => t('Auto-Scroll Menu'),
       '#default_value' => isset($menu_content['autoscroll']) ? $menu_content['autoscroll']: 0,
       '#description' => t('Make your menu automatically scroll to stay on screen.'),       
      ); 
		}
	}
  return $form_new;				
}