<?php
/* $Id$ */

/**
 * @file
 * Maximenu Effect - Slideup
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Slide Up
 *
 */
function _maximenu_slideup($menu = NULL) {
  $link_slide_js = "
    linkHeight = $('#menu-" . $menu . " .leaf').height();
  
    $('#menu-" . $menu . " .leaf').css('height', linkHeight + 'px');
    $('#menu-" . $menu . " .leaf .link').css('overflow', 'hidden').css('height', 0);      
	  $('#menu-" . $menu . " .leaf').prepend('<span class=\"link-slideup\"></span>'); 

	  $('#menu-" . $menu . " .leaf').each(function() { 
		  var textLink = $(this).find('.link').text(); 
		  $(this).find('span.link-slideup').show().text(textLink); 
	  }); 

	  $('#menu-" . $menu . " .leaf').hover(
	    function() { 
	      $(this).find('span.link-slideup').stop().animate({ lineHeight: 0, height: 0 }, 250);
	      $(this).find('.link').stop().animate({ height: linkHeight }, 250); 
	    },
	    function() { 
	      $(this).find('span.link-slideup').stop().animate({ lineHeight: linkHeight, height: linkHeight }, 250);
	      $(this).find('.link').stop().animate({ height: 0 }, 250);
	  });
  ";
  drupal_add_css(MAXIMENU_PLUGINS . '/effects/slideup/slideup.css');

  return $link_slide_js;
}
