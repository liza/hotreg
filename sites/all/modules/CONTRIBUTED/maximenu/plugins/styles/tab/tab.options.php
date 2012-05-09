<?php
/* $Id$ */

/**
 * @file
 * Maximenu Style - Tab
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Loads options to Maximenu admin
 *
 */  
function _maximenu_tab_options($menu_content = array(), $settings = array()) {

	$form_new = array();

  // Skin
	// None
	
	// Style - Tab
  $form_new['action'] = array(
    '#type' => 'select',
    '#title' => t('Mouse Action'),
    '#options' => array(
		  'hover_fast' => t('Hover (Fast)'),
			'hover_slow' => t('Hover (Slow)'),
			'click_fast' => t('Click (Fast)'),
			'click_slow' => t('Click (Slow)'),
		),
		'#default_value' => isset($menu_content['action']) ? $menu_content['action']: 'hover_fast',
  );   
  $form_new['tab_scroller'] = array(
    '#type'  => 'select',
    '#title' => t('Scroll'),
	  '#options' => array(
		  'no' => t('No'),  
			'scrollh' => t('Horizontal Scroll '), 
			'scrollv' => t('Vertical Scroll '),
		),
    '#default_value' => isset($menu_content['tab_scroller']) ? $menu_content['tab_scroller']: 'no',
    '#description' => t('Scroller'),   
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