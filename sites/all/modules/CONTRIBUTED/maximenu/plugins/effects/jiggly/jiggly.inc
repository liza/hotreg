<?php
/* $Id$ */

/**
 * @file
 * Maximenu Effect - Jiggly
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Animated link giggle
 *
 */
function _maximenu_jiggly($menu = NULL) {  
  
  $link_jiggly_js = "
	    // creates the target paths
	    var list_elements = '#menu-" . $menu . "' + ' li.leaf';
	    var link_elements = list_elements + ' .link';
	    var pad_out = 25;
	    var pad_in = 15;
	    var time = 150;
	    var multiplier = .8;
	    
  	  // initiates the timer used for the sliding animation
  	  var timer = 0;
	
	    // creates the jiggle animation for all list elements 
	    $(list_elements).each(function(i){
  		  // margin left = - ([width of element] + [total vertical padding of element])
	  	  $(this).css('margin-left','-180px');
	  	  // updates timer
	  	  timer = (timer*multiplier + time);
	  	  $(this).animate({ marginLeft: '0' }, timer);
	  	  $(this).animate({ marginLeft: '15px' }, timer);
	  	  $(this).animate({ marginLeft: '0' }, timer);
	    });

  	  // creates the hover-jiggle effect for all link elements 		
  	  $(link_elements).each(function(i){
	  	  $(this).hover(
		      function(){
			      $(this).animate({ paddingLeft: pad_out }, 150);
		      },		
		      function(){
		      	$(this).animate({ paddingLeft: pad_in }, 150);
		    });
	    });
    ";
  return $link_jiggly_js;  
}
    

