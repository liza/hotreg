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
 * Loads styles and js
 *
 */
function _maximenu_accordion_render($menu_content = array()) {

  // Skin    
  $skin          = isset($menu_content['skin']) ? $menu_content['skin']: FALSE;
  drupal_add_css(MAXIMENU_PLUGINS . '/styles/accordion/accordion.css');
  if ($skin) drupal_add_css(MAXIMENU_PLUGINS . '/skins/' . $skin . '/' . $skin . '.css');
		
  // Style - Accordion
  $menu      = _maximenu_string($menu_content['title']); 
  $autostart = isset($menu_content['autostart']) ? $menu_content['autostart']: 'true';  
  $speed     = isset($menu_content['speed']) ? $menu_content['speed']: 2000;  
  $slidenum  = isset($menu_content['slidenum']) ? $menu_content['slidenum']: 'false';  

  $scripts = _maximenu_accordion_js ($menu, $autostart, $speed, $slidenum); 	
  
	// Effects
	// None
	
  // Others
  $autoscroll    = isset($menu_content['autoscroll']) ? 1: 0;                       
  if ($autoscroll) $scripts .=_maximenu_autoscroll($menu);
	
	return $scripts;
}


/**
 * Mouse actions
 *
 */
function _maximenu_accordion_js ($menu = '', $autostart = 'true', $speed = '2000', $slidenum = 'false') {

  $accordion_js = "
    var height = $('#maximenu-" . $menu . "').height();
    var width = $('#maximenu-" . $menu . "').width();

    $('#maximenu-" . $menu . " dl.easy-accordion').css('width', width + 'px').css('height', height + 'px');
    $('#maximenu-" . $menu . " dl.easy-accordion>dt').removeClass('active');  
    $('#maximenu-" . $menu . " dl.easy-accordion>dd').removeClass('active');  
    $('#maximenu-" . $menu . " dl.easy-accordion>dt:first-child').addClass('active');  
    $('#maximenu-" . $menu . " dl.easy-accordion>dd:first-child').addClass('active');  
                         
    $('#maximenu-" . $menu . "').easyAccordion({
      autoStart: " . $autostart . ",
      slideInterval: " . $speed . ",
      slideNum: " . $slidenum . "
    });
  ";
  drupal_add_js(MAXIMENU_PLUGINS . '/styles/accordion/jquery.easyAccordion.js'); 
  return $accordion_js;       
}

