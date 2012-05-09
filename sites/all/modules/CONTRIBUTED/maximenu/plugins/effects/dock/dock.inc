<?php
/* $Id$ */

/**
 * @file
 * Maximenu Effect - Dock
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Dock jquery plugin
 *
 */
function _maximenu_dock($menu = '') {
  $add_js = "$('#menu-" . $menu . " .link img').resizeOnApproach();";
  drupal_add_js(MAXIMENU_PLUGINS . '/effects/dock/jquery.resizeOnApproach.min.js'); 
  drupal_add_css(MAXIMENU_PLUGINS . '/effects/dock/dock.css');

  return $add_js;
}
