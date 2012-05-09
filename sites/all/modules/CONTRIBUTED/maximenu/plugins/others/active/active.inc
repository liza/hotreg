<?php
/* $Id$ */

/**
 * @file
 * Maximenu Other - Active
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Make menus active when submenus are active
 *
 */
function _maximenu_active_js($menu = NULL, $animated_bg = 0, $style = NULL) {

  $active     = 'active-trail';
  $lava       = 'selectedLava';
  $class = $animated_bg ? $lava: $active;
  
  $active_js = "
      $.each($('#menu-" . $menu . " .leaf'),function() {

        var active = $('.menu a', this).hasClass('active');
        var active_trail = $('.menu a', this).hasClass('active-trail');
        
        if ((active == true) || (active_trail == true)) {
          $('#menu-" . $menu . " .leaf').removeClass('" . $class . "');
          $(this).addClass('" . $class . "');
        }
      });
    ";

  $active_tab_js = "
      $.each($('#menu-" . $menu . " .leaf'),function() {
        var leafId = $(this).attr('id');
        var tabContentId = leafId.replace('leaf', 'tab-content');
        var active_tab = $('.menu a', '.maximenu-tab-content #' + tabContentId).hasClass('active');

        var active = $('.menu a', this).hasClass('active');
        
        if ((active_tab == true) || (active == true)) {
          $('#menu-" . $menu . " .leaf').removeClass('" . $class . "');
          $(this).addClass('" . $class . "');
        }
      });
   ";
  
  if ($style == 'tab') {    
    return $active_tab_js;
  }
  else {
    return $active_js;  
  }
} 
