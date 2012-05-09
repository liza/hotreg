<?php
/* $Id$ */

/**
 * @file
 * Maximenu Style - Modal
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Loads options to Maximenu admin
 *
 */  
function _maximenu_modal_options($menu_content = array(), $settings = array()) {

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
	
	// Style - Modal
  $form_new['action'] = array(
    '#type' => 'select',
    '#title' => t('Mouse Action'),
    '#options' => array('click' => t('Click'), 'hover' => t('Hover')),
		'#default_value' => isset($menu_content['action']) ? $menu_content['action']: 'click',
  );
  $form_new['delay'] = array(
    '#type'  => 'textfield',
    '#title' => t('Hover Delay'),
    '#default_value' => isset($menu_content['delay']) ? $menu_content['delay']: 1000,
    '#description' => t('This is used for Hover (fade/slow), 1 sec = 1000'),   
  );

	// Effects
  $effects_options  = isset($settings['effects']) ? $settings['effects']: '';
	if (!empty($effects_options)) {
    if (isset($effects_options['dock'])) {
      $form_new['dock'] = array(
        '#type'  => 'checkbox',
        '#title' => t('Dock'),
        '#default_value' => isset($menu_content['dock']) ? $menu_content['dock']: 0,
        '#description' => t('Mac-like control panel'),   
      ); 
		}
    if (isset($effects_options['jiggly'])) {
      $form_new['jiggly'] = array(
        '#type'  => 'checkbox',
        '#title' => t('Jiggly Links'),
        '#default_value' => isset($menu_content['jiggly']) ? $menu_content['jiggly']: 0,
        '#description' => t('Make the links jiggle on hover.'),       
      );   
  	}
    if (isset($effects_options['lavalamp'])) {
      $form_new['lavalamp'] = array(
        '#type'  => 'checkbox',
        '#title' => t('Lavalamp (Animated Background)'),
        '#default_value' => isset($menu_content['lavalamp']) ? $menu_content['lavalamp']: 0,
        '#description' => t('Links with sliding background'),   
      );
			$defaults = '
        target: ".leaf",
        container: "li",
        fx: "swing",
        speed: 700,  
        click: function(){return true}, 
        startItem: "",
        includeMargins: false,
        autoReturn: true,
        returnDelay: 0,
        setOnClick: true,
        homeTop:0,
        homeLeft:0,
        homeWidth:0,
        homeHeight:0,
        returnHome:false,
        autoResize:false
			'; 
      $form_new['defaults'] = array(
        '#type'  => 'textarea',
        '#title' => t('Defaults'),
		    '#rows'  => 10, 
        '#default_value' => isset($menu_content['defaults']) ? $menu_content['defaults']: $defaults,
      );
		}
    if (isset($effects_options['slideup'])) {
      $form_new['slideup'] = array(
        '#type'  => 'checkbox',
        '#title' => t('Slide Up (Animated Link)'),
        '#default_value' => isset($menu_content['slideup']) ? $menu_content['slideup']: 0,
        '#description' => t('Links sliding vertically'),   
      ); 
		}
  }
	
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
  	if (!empty($others_options['longmenu'])) {
      $form_new['longmenu'] = array(
        '#type'  => 'select',
        '#title' => t('Long horizontal menu options'),
        '#options' => array('' => 'No', 'hover' => 'Hover', 'prev_next' => 'Prev/Next'),    
        '#default_value' => isset($menu_content['longmenu']) ? $menu_content['longmenu']: '',
        '#description' => t('
           Set this menu to scroll if it won\'t fit the container.
           <ul>
             <li><strong>Hover</strong> - long menus will scroll on hover</li>
             <li><strong>Prev/Next</strong> - adds prev and next buttons for long menus</li>
           </ul>
           <strong>Note:</strong> If used in menu with dropdown style, it will hide all its attached blocks.
        '),            
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