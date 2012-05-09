<?php
/* $Id$ */

/**
 * @file
 * Maximenu Style - Roundabout
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Loads options to Maximenu admin
 *
 */  
function _maximenu_roundabout_options($menu_content = array(), $settings = array()) {

	$form_new = array();
 
	// Skin
  $skin_options  = isset($settings['skins']) ? $settings['skins']: '';
	if (!empty($skin_options)) {
    $form_new['skin'] = array(
      '#type'  => 'select',
      '#title' => t('Skin'),
      '#options' => $skin_options,    
      '#default_value' => isset($menu_content['skin']) ? $menu_content['skin']: 'bubble',
    ); 
	}
  
	// Style - Roundabout
  $defaults = '
  bearing: 0.0,
  tilt: 0.0,
  minZ: 100,
  maxZ: 280,
  minOpacity: 0.4,
  maxOpacity: 1.0,
  minScale: 0.4,
  maxScale: 1.0,
  duration: 600,
  btnNext: null,
  btnNextCallback: function() {},
  btnPrev: null,
  btnPrevCallback: function() {},
  btnToggleAutoplay: null,
  btnStartAutoplay: null,
  btnStopAutoplay: null,
  easing: "swing",
  clickToFocus: true,
  clickToFocusCallback: function() {},
  focusBearing: 0.0,
  shape: "lazySusan",
  debug: false,
  childSelector: "li",
  startingChild: null,
  reflect: false,
  floatComparisonThreshold: 0.001,
  autoplay: false,
  autoplayDuration: 1000,
  autoplayPauseOnHover: false,
  autoplayCallback: function() {},
  autoplayInitialDelay: 0,
  enableDrag: false,
  dropDuration: 600,
  dropEasing: "swing",
  dropAnimateTo: "nearest",
  dropCallback: function() {},
  dragAxis: "x",
  dragFactor: 4,
  triggerFocusEvents: true,
  triggerBlurEvents: true,
  responsive: false';
		
  $form_new['defaults'] = array(
    '#type'  => 'textarea',
    '#title' => t('Defaults'),
		'#rows'  => 20, 
    '#default_value' => isset($menu_content['defaults']) ? $menu_content['defaults']: $defaults,
  ); 	
	// Effects
	// None
	
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