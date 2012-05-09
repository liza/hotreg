<?php
/* $Id$ */

/**
 * @file
 * Maximenu Effect - Lavalamp
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Lavalamp
 *
 */
function _maximenu_lavalamp($menu = NULL, $defaults = NULL) {
  //$lavalamp_js = "$('ul#menu-" . $menu . "').lavaLamp({ target: '.leaf', container: 'li', speed: 700 });";
  $lavalamp_js = "$('ul#menu-" . $menu . "').lavaLamp({ " . $defaults . " });";
  drupal_add_js(MAXIMENU_PLUGINS . '/effects/lavalamp/jquery.lavalamp.min.js');   
  drupal_add_css(MAXIMENU_PLUGINS . '/effects/lavalamp/lavalamp.css');

  return $lavalamp_js;  
}
