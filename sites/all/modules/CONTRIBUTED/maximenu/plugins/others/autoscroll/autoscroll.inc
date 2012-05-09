<?php
/* $Id$ */

/**
 * @file
 * Maximenu Other - Autoscroll
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Animated floating menu
 *
 */
function _maximenu_autoscroll($menu = NULL) {    

  $scroll_float_js = "
      var topYloc = null;

      $(window).scroll(function () { 
	      var scrollTop = $(document).scrollTop();
	      scrollTop = parseInt(scrollTop);
	
	      var offset = topYloc+scrollTop+'px';  
	      $('#maximenu-" . $menu . "').animate({top:offset},{duration:500,queue:false});
      });

	    topYloc = parseInt($('#maximenu-" . $menu . "').css('top').substring(0,$('#maximenu-" . $menu . "').css('top').indexOf('px')));
    ";
  return $scroll_float_js;  
}
