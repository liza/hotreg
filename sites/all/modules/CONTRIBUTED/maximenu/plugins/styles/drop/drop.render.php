<?php
/* $Id$ */

/**
 * @file
 * Maximenu Style - Drop
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Loads styles and js
 *
 */
function _maximenu_drop_render($menu_content = array()) {

  // Skin
  $skin = isset($menu_content['skin']) ? $menu_content['skin']: FALSE;
  // add css per  Maximenu     
  drupal_add_css(MAXIMENU_PLUGINS . '/styles/drop/drop.css');
  if ($skin) drupal_add_css(MAXIMENU_PLUGINS . '/skins/' . $skin . '/' . $skin . '.css');
		
  // Style - Drop
  $menu     = _maximenu_string($menu_content['title']); 
  $action   = isset($menu_content['action']) ? $menu_content['action']: 'hover_fast';  
  $displace = isset($menu_content['displace']) ? $menu_content['displace']: 0;  
  $delay    = isset($menu_content['delay']) ? $menu_content['delay']: 1000;  
  $fadeout  = isset($menu_content['fadeout']) ? $menu_content['fadeout']: 1;          

  $scripts = _maximenu_drop_js($menu, $action, $displace, $delay, $fadeout); 	

  // Effects
  $dock     = isset($menu_content['dock']) ? 1: 0;
  $jiggly   = isset($menu_content['jiggly']) ? 1: 0; 
  $lavalamp = isset($menu_content['lavalamp']) ? 1: 0; 
  $lavalamp_defaults = isset($menu_content['lavalamp_defaults']) ? $menu_content['lavalamp_defaults']: ''; 
  $slideup  = isset($menu_content['slideup']) ? 1: 0; 

  if ($dock) $scripts       .=_maximenu_dock($menu);
  if ($lavalamp) $scripts   .=_maximenu_lavalamp($menu, $lavalamp_defaults); 
  if ($slideup) $scripts    .=_maximenu_slideup($menu); 
  if ($jiggly) $scripts     .=_maximenu_jiggly($menu);
	  
	// Others
  $active        = isset($menu_content['active']) ? $menu_content['active']: 0;  
  $longmenu      = isset($menu_content['longmenu']) ? $menu_content['longmenu']: FALSE;  
  $autoscroll    = isset($menu_content['autoscroll']) ? 1: 0;                       
	
  if ($active) $scripts     .= _maximenu_active_js($menu, $lavalamp, 'drop'); 
  if ($longmenu) $scripts   .=_maximenu_longmenu_js($menu, $longmenu); 
  if ($autoscroll) $scripts .=_maximenu_autoscroll($menu);
        	
	return $scripts;
}


/**
 * Mouse actions
 *
 */
function _maximenu_drop_js($menu = '', $action = 'hover_fast', $displace = 0, $delay = 1000, $fadeout = 1) {

  $displace_menu = ($displace == 1) ? "$('#maximenu-" . $menu . " li.leaf .maximenu-content').css('position', 'relative').addClass('maximenu-displace');": '';

  if ($action == 'click_fast') {
    $click_action = "
        $(this).parent().siblings().children('.maximenu-content').hide();        
        $(this).siblings('.maximenu-content').toggle().pause();";
  }
  elseif ($action == 'click_slow') {
    $click_action = "
        $(this).parent().siblings().children('.maximenu-content').slideUp('slow');        
        $(this).siblings('.maximenu-content').toggle('slow').pause();";        
  }
  else {
    $click_action = "";  
  }
  
  $click = $displace_menu . "     
      $('#maximenu-" . $menu . " li.leaf .maximenu-content').removeClass('maximenu-content-nofade');
      $('#maximenu-" . $menu . " li.leaf .link').click(function() {
				if($(this).parent().hasClass('open')) {
				  $(this).parent().removeClass('open');
				}
				else {
				  $('#maximenu-" . $menu . " li.leaf').removeClass('open');
				  $(this).parent().addClass('open');
				}	
        " . $click_action . "       
        return false;
      });
    ";

	$fadeout_out = ($fadeout == 1) ? '.fadeOut()': '';
	            
  $hover_fade = $displace_menu . "     
      $('#maximenu-" . $menu . " li.leaf .maximenu-content').removeClass('maximenu-content-nofade');
      //$('#maximenu-" . $menu . " li.leaf').hover(FadeIn,FadeOut); //native jquery
      $('#maximenu-" . $menu . " li.leaf').hoverIntent({
        over: FadeIn,
        timeout: " . $delay . ",
        out: FadeOut
      }); 
      function FadeIn(){ $('.maximenu-content.closed', this).fadeIn(); }
      function FadeOut(){ $('.maximenu-content.closed', this)" . $fadeout_out . "; }  
    ";

  $hover = $displace_menu;

 
  if (($action == 'click_fast') || ($action == 'click_slow')) { 
    return $click; 
  }
  else {
    if ($action == 'hover_slow') {
      drupal_add_js(MAXIMENU_PLUGINS . '/styles/drop/jquery.hoverIntent.minified.js'); 
      return $hover_fade; 
    }
    else {
      if ($displace == 1) return $hover; 
    }  
  } 
}


