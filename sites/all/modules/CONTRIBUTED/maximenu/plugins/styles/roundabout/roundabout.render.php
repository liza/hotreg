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
 * Loads styles and js
 *
 */
function _maximenu_roundabout_render($menu_content = array()) {
	
	// Skin
  $skin = isset($menu_content['skin']) ? $menu_content['skin']: FALSE;
  drupal_add_css(MAXIMENU_PLUGINS . '/styles/roundabout/roundabout.css');
  if ($skin) drupal_add_css(MAXIMENU_PLUGINS . '/skins/' . $skin . '/' . $skin . '.css');
	
	// Style - Roundabout
  $menu = _maximenu_string($menu_content['title']); 
	$defaults = !empty($menu_content['defaults']) ? $menu_content['defaults']: '';
	
  $scripts = _maximenu_roundabout_js ($menu, $defaults); 	
	
	// Effects
	// None
	
	// Others
  $autoscroll = isset($menu_content['autoscroll']) ? 1: 0;                       

  if ($autoscroll) $scripts .=_maximenu_autoscroll($maximenu_name);
	
	return $scripts;
}


/**
 * Mouse actions
 *
 */
function _maximenu_roundabout_js ($menu = '', $defaults) {
  
  $roundabout_js = "$('#menu-" . $menu . "').roundabout({" . $defaults . "});";
          
  drupal_add_js(MAXIMENU_PLUGINS . '/styles/roundabout/jquery.roundabout.min.js'); 
  return $roundabout_js;       
}

