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
 * Loads styles and js
 *
 */
function _maximenu_modal_render($menu_content = array()) {
	
	// Skin
  $skin = isset($menu_content['skin']) ? $menu_content['skin']: FALSE;
  drupal_add_css(MAXIMENU_PLUGINS . '/styles/modal/modal.css');
  if ($skin) drupal_add_css(MAXIMENU_PLUGINS . '/skins/' . $skin . '/' . $skin . '.css');
	
	// Style - Modal
  $menu   = _maximenu_string($menu_content['title']); 
  $action = isset($menu_content['action']) ? $menu_content['action']: 'click';  
  $delay  = isset($menu_content['delay']) ? $menu_content['delay']: 1000;  

  $scripts = _maximenu_modal_js($menu, $action, $delay);
	
	// Effects
  $dock     = isset($menu_content['float_options']['dock']) ? 1: 0;
  $jiggly   = isset($menu_content['jiggly']) ? 1: 0; 
  $lavalamp = isset($menu_content['lavalamp']) ? 1: 0; 
  $lavalamp_defaults = isset($menu_content['lavalamp_defaults']) ? $menu_content['lavalamp_defaults']: ''; 
  $slideup  = isset($menu_content['slideup']) ? 1: 0; 

  if ($dock) $scripts .=_maximenu_dock($menu);	
  if ($jiggly) $scripts .=_maximenu_jiggly($menu);
  if ($lavalamp) $scripts .=_maximenu_lavalamp($menu, $lavalamp_defaults); 
  if ($slideup) $scripts .=_maximenu_slideup($menu); 	
	
	// Others
  $active     = isset($menu_content['active']) ? $menu_content['active']: 0;  
  $autoscroll = isset($menu_content['autoscroll']) ? 1: 0;                       
  $longmenu   = isset($menu_content['longmenu']) ? $menu_content['longmenu']: FALSE;  
	
  if ($active) $scripts .= _maximenu_active_js($menu, $lavalamp, 'modal'); 
  if ($autoscroll) $scripts .=_maximenu_autoscroll($menu);
  if ($longmenu) $scripts .=_maximenu_longmenu_js($menu, $longmenu);         

	return $scripts;
}


/**
 * Mouse actions
 *
 */
function _maximenu_modal_js ($menu = '', $action = 'click', $delay = 1000) {
  
  $style_modal_js = "   
	  $('#maximenu-" . $menu . " .link[rel=maximenu-modal]')." . $action . "(function(e) {
	    // prevent default link behavior
	    e.preventDefault();
		
	    // get anchor id
	    var id = $(this).attr('href');

	    // mask height and width
	    var maskHeight = $(document).height();
	    var maskWidth = $(window).width();
	
	    $('#maximenu-mask').css({'width': maskWidth, 'height': maskHeight});
		
	    // fade effect	
	    $('#maximenu-mask').fadeIn(" . $delay . ");	
	    $('#maximenu-mask').fadeTo('slow',0.75);	
	
	    // maximenu-modal-content height and width
	    var windowHeight = $(window).height();
	    var windowWidth = $(window).width();
              
	    // set maximenu-modal-content to center
	    $(id).css('top',  windowHeight/2-$(id).height()/2);
	    $(id).css('left', windowWidth/2-$(id).width()/2);
	
	    // fade effect to window
	    $(id).fadeIn(" . $delay . "); 
	  }, function() {
	    // do nothing
	  });
	
	  // if close button is clicked
	  $('.maximenu-modal-content .maximenu-close').click(function() {
	    $('#maximenu-mask').hide();
	    $('.maximenu-modal-content').hide();
	  });		
	
	  // if maximenu-mask is clicked
	  $('#maximenu-mask').click(function() {
		  $(this).hide();
		  $('.maximenu-modal-content').hide();
	  });			
  ";
  return $style_modal_js;     
}

