<?php
/* $Id$ */

/**
 * @file
 * Maximenu Style - Tab
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Loads styles and js
 *
 */
function _maximenu_tab_render($menu_content = array()) {
	
	// Skin
  $skin          = isset($menu_content['skin']) ? $menu_content['skin']: FALSE;
  drupal_add_css(MAXIMENU_PLUGINS . '/styles/tab/tab.css');
  if ($skin) drupal_add_css(MAXIMENU_PLUGINS . '/skins/' . $skin . '/' . $skin . '.css');
	
	// Style - Tab
  $menu         = _maximenu_string($menu_content['title']); 	
  $action       = isset($menu_content['action']) ? $menu_content['action']: 'hover_fast'; 
  $tab_scroller = isset($menu_content['tab_scroller']) ? $menu_content['tab_scroller']: 'no';  
 
  $scripts = _maximenu_tab_js($menu, $action, $tab_scroller); 		
	
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

  if ($active) $scripts .= _maximenu_active_js($menu, $lavalamp, 'tab'); 
  if ($autoscroll) $scripts .=_maximenu_autoscroll($menu);      
  if ($longmenu) $scripts .=_maximenu_longmenu_js($menu, $longmenu); 
	
	return $scripts;
}


/**
 * Mouse actions
 *
 */
function _maximenu_tab_js ($menu = '', $action = 'hover_fast', $tab_scroller = 'no') {

  if (($action == 'hover_fast') || ($action == 'click_fast')) {
    $tab_action = "
      $('#maximenu-" . $menu . " .maximenu-tab-content .tab-content').addClass('tab-content-hide');              
      $('#maximenu-" . $menu . " #' + tabContentId).removeClass('tab-content-hide');";
  }
  elseif (($action == 'hover_slow') || ($action == 'click_slow')) {
    $tab_action = "
      $('#maximenu-" . $menu . " .maximenu-tab-content .tab-content').addClass('tab-content-hide').hide();              
      $('#maximenu-" . $menu . " #' + tabContentId).fadeIn('slow').removeClass('tab-content-hide');";    
  }
  else {
    $tab_action = "";  
  }

  $tab_hover_js = "
    $('#maximenu-" . $menu . " ul.menu li.leaf').removeClass('active');          
    $('#maximenu-" . $menu . " ul.menu li.leaf:first-child').addClass('active');          
    $('#maximenu-" . $menu . " .maximenu-tab-content div:first-child').removeClass('tab-content-hide'); 
               
    $('#maximenu-" . $menu . " .leaf .link').hover(
      function () {
        var leafId = $(this).parent().attr('id');
        var tabContentId = leafId.replace('leaf', 'tab-content');
        $('#maximenu-" . $menu . " ul.menu li.leaf').removeClass('active');          
        $(this).parent().addClass('active');       
        " . $tab_action . " 
      }, function () {
        //do nothing
    });
  ";
    
  $tab_click_js = "
    $('#maximenu-" . $menu . " ul.menu li.leaf').removeClass('active');          
    $('#maximenu-" . $menu . " ul.menu li.leaf:first-child').addClass('active');          
    $('#maximenu-" . $menu . " .maximenu-tab-content div:first-child').removeClass('tab-content-hide'); 
               
    $('#maximenu-" . $menu . " .leaf .link').click(function () {
      var leafId = $(this).parent().attr('id');
      var tabContentId = leafId.replace('leaf', 'tab-content');
      $('#maximenu-" . $menu . " ul.menu li.leaf').removeClass('active');          
      $(this).parent().addClass('active');            
      " . $tab_action . "      
    });
  ";  
  
  $flow = '';  
  if ($tab_scroller == 'scrollv')    $flow = "$('#maximenu-" . $menu . " .maximenu-tab-content-inner').css('height', totalHeight + 'px');"; 
  if ($tab_scroller == 'scrollh')    $flow = "$('#maximenu-" . $menu . " .maximenu-tab-content-inner').css('width', totalWidth + 'px');"; 
  if ($action == 'hover_fast') $speed = 1000;
  if ($action == 'hover_slow') $speed = 2000;
  if ($action == 'click_fast') $speed = 750;
  if ($action == 'click_slow') $speed = 1500;   
    
  $tab_scroll_hover_js = "
      var contentNum = $('#maximenu-" . $menu . " .maximenu-tab-content-inner').children().size();
      var height = $('#maximenu-" . $menu . " .maximenu-tab-content').height();
      var width = $('#maximenu-" . $menu . " .maximenu-tab-content').width();
      var totalHeight = height * contentNum;
      var totalWidth = width * contentNum;

      $('#maximenu-" . $menu . " .tab-content').css('height', height + 'px');
      $('#maximenu-" . $menu . " .tab-content').css('width', width + 'px');
      " . $flow . "
                        
      $('#maximenu-" . $menu . " ul.menu li.leaf').removeClass('active');          
      $('#maximenu-" . $menu . " ul.menu li.leaf:first-child').addClass('active');          
      $('#maximenu-" . $menu . " .maximenu-tab-content .tab-content').removeClass('tab-content-hide'); 
               
      $('#maximenu-" . $menu . " .leaf .link').hover(
        function () {
          var leafId = $(this).parent().attr('id');
          var tabContentId = leafId.replace('leaf', 'tab-content');
                        
          $('#maximenu-" . $menu . " ul.menu li.leaf').removeClass('active');          
          $(this).parent().addClass('active');          

          $('#maximenu-" . $menu . " .maximenu-tab-content').scrollTo($('#' + tabContentId), " . $speed . ");
        }, function () {
          // do nothing
      });

  ";

  $tab_scroll_click_js = "
      var contentNum = $('#maximenu-" . $menu . " .maximenu-tab-content-inner').children().size();
      var height = $('#maximenu-" . $menu . " .maximenu-tab-content').height();
      var width = $('#maximenu-" . $menu . " .maximenu-tab-content').width();
      var totalHeight = height * contentNum;
      var totalWidth = width * contentNum;

      $('#maximenu-" . $menu . " .tab-content').css('height', height + 'px');
      $('#maximenu-" . $menu . " .tab-content').css('width', width + 'px');
      " . $flow . "
                        
      $('#maximenu-" . $menu . " ul.menu li.leaf').removeClass('active');          
      $('#maximenu-" . $menu . " ul.menu li.leaf:first-child').addClass('active');          
      $('#maximenu-" . $menu . " .maximenu-tab-content .tab-content').removeClass('tab-content-hide'); 
               
      $('#maximenu-" . $menu . " .leaf .link').click(function () {
        var leafId = $(this).parent().attr('id');
        var tabContentId = leafId.replace('leaf', 'tab-content');
                        
        $('#maximenu-" . $menu . " ul.menu li.leaf').removeClass('active');          
        $(this).parent().addClass('active');          

        $('#maximenu-" . $menu . " .maximenu-tab-content').scrollTo($('#' + tabContentId), " . $speed . ");
      });
  "; 
          
  if (($action == 'click_fast') || ($action == 'click_slow')) { 
    if ($tab_scroller == 'no') { 
      return $tab_click_js; 
    }     
    else {
      drupal_add_js(MAXIMENU_PLUGINS . '/styles/tab/jquery.scrollTo.min.js'); 
      return $tab_scroll_click_js;   
    } 
  }
  else {
    if ($tab_scroller == 'no') { 
      return $tab_hover_js; 
    }     
    else {
      drupal_add_js(MAXIMENU_PLUGINS . '/styles/tab/jquery.scrollTo.min.js'); 
      return $tab_scroll_hover_js;   
    } 
  } 
}

