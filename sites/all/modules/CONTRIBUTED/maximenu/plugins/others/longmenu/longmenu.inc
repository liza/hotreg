<?php
/* $Id$ */

/**
 * @file
 * Maximenu Other - Long Menu
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Scroll long menu if it won't fit the container
 *
 */
function _maximenu_longmenu_js($menu = NULL, $longmenu = NULL) {

  $longmenu_hover_scroll_js = "
    if ($('#maximenu-" . $menu . "').hasClass('maximenu-row')) {
      var wrapper = $('#maximenu-" . $menu . " .menu-ul-wrapper'), 
          wrapper_width = wrapper.width(),
          ul = $('#menu-" . $menu . "'), 
          ul_width = 0;
        
      $('#menu-" . $menu . " .leaf').each(function(index) {
        ul_width += parseInt($(this).width(), 10);
      });
        
      // this will only be triggered when links won't fit inside the wrapper
      if (ul_width > wrapper_width) {

        var li_first = ul.find('li:first-child').width(),
            li_last = ul.find('li:last-child').width();
            
        // scroll adjustment to end link
        ul.width(ul_width + 30 + 'px')
        
        wrapper.css({overflow: 'hidden'});
        
        wrapper.mousemove(function(e){
          var wrapper_offset = wrapper.offset();
          var mouse = (e.pageX - wrapper_offset.left) * (ul_width - wrapper_width) / wrapper_width;

          // scroll adjustments for both ends
          if(mouse < li_first) mouse = (e.pageX - wrapper_offset.left - li_first) * (ul_width - wrapper_width) / wrapper_width;
          if(mouse > li_first) mouse = (e.pageX - wrapper_offset.left) * (ul_width - wrapper_width + li_last + 15) / wrapper_width;

          wrapper.scrollLeft(mouse);
        });
      }
    }
    else if ($('#maximenu-" . $menu . "').hasClass('maximenu-column')) {
      var wrapper = $('#maximenu-" . $menu . " .menu-ul-wrapper'), 
          wrapper_height = wrapper.height(),
          ul = $('#menu-" . $menu . "'), 
          ul_height = 0;

      $('#menu-" . $menu . " .leaf').each(function(index) {
        ul_height += parseInt($(this).height(), 10);
      });
      // this will only be triggered when links won't fit inside the wrapper
      if (ul_height > wrapper_height) {
   
        var li_first = ul.find('li:first-child').height(),
            li_last = ul.find('li:last-child').height();

        // scroll adjustment to end link
        ul.height(ul_height + 30 + 'px')
                           
        wrapper.css({overflow: 'hidden'});
        wrapper.mousemove(function(e){
          var wrapper_offset = wrapper.offset();
          var mouse = (e.pageY - wrapper_offset.top) * (ul_height - wrapper_height) / wrapper_height;

          // scroll adjustments for both ends
          if(mouse < li_first) mouse = (e.pageY - wrapper_offset.top - 5) * (ul_height - wrapper_height) / wrapper_height;

          wrapper.scrollTop(mouse);
        });
      }
    }";

  $longmenu_prev_next_scroll_js = "
      if ($('#maximenu-" . $menu . "').hasClass('maximenu-row')) {
        var wrapper = $('#maximenu-" . $menu . " .menu-ul-wrapper'), 
            wrapper_width = wrapper.width(),
            ul = $('#menu-" . $menu . "'), 
            ul_width = 0;
        
        $('#menu-" . $menu . " .leaf').each(function(index) {
          ul_width += parseInt($(this).width(), 10);
        });
  
        // this will only be triggered when links won't fit inside the wrapper
        if (ul_width > wrapper_width) {

          var wrapper_height = 30,
              li_first = ul.find('li:first-child').width(),
              li_last = ul.find('li:last-child').width();
                    
          wrapper.height(wrapper_height + 'px');

          $('#menu-" . $menu . "-ul-wrapper').css('margin', '0 30px');        
          $('#menu-" . $menu . "').css('position', 'absolute');
        
          // scroll adjustment to end link
          ul.width(ul_width + 30 + 'px')
                
          wrapper.css({overflow: 'hidden'});
        
          $('#maximenu-" . $menu . "').prepend('<div class=\"menu-scroller menu-previous\"><</div>');
          $('#maximenu-" . $menu . "').append('<div class=\"menu-scroller menu-next\">></div>');
        
          $('#maximenu-" . $menu . "').serialScroll({
            target:'#menu-" . $menu . "-ul-wrapper',
            items:'li.leaf', 
            prev:'div.menu-previous',
            next:'div.menu-next',
            axis:'xy',
            navigation:'',
            duration: 250,
            force:true,
            onBefore:function( e, elem, \$pane, \$items, pos ){
              e.preventDefault();
              if( this.blur ) this.blur();
            },
            onAfter:function( elem ){
              //'this' is the element being scrolled (\$pane) not jqueryfied
            }
          });
        }
      }
      else if ($('#maximenu-" . $menu . "').hasClass('maximenu-column')) {
        //$('#maximenu-" . $menu . " .menu-ul-wrapper').height(300);
        var wrapper = $('#maximenu-" . $menu . " .menu-ul-wrapper'), 
            wrapper_height = wrapper.height(),
            ul = $('#menu-" . $menu . "'), 
            ul_height = 0;

        $('#menu-" . $menu . " .leaf').each(function(index) {
          ul_height += parseInt($(this).height(), 10);
        });
  
        // this will only be triggered when links won't fit inside the wrapper
        if (ul_height > wrapper_height) {

          var wrapper_width = wrapper.width(),
              li_first = ul.find('li:first-child').height(),
              li_last = ul.find('li:last-child').height();

          $('#menu-" . $menu . "').css('position', 'absolute');
                      
          // scroll adjustment to end link
          ul.height(ul_height + 30 + 'px');
                  
          wrapper.css({overflow: 'hidden'});

          $('#maximenu-" . $menu . "').prepend('<div class=\"menu-scroller menu-previous\">^</div>');
          //$('#maximenu-" . $menu . "').append('<div class=\"menu-scroller menu-next\">v</div>');
          $('<div class=\"menu-scroller menu-next\">v</div>').insertAfter(wrapper);
                    
          $('#maximenu-" . $menu . " .menu-scroller').width(wrapper_width);
          
          $('#maximenu-" . $menu . "').serialScroll({
            target:'#menu-" . $menu . "-ul-wrapper',
            items:'li.leaf', 
            prev:'div.menu-previous',
            next:'div.menu-next',
            axis:'xy',
            navigation:'',
            duration: 250,
            force:true,
            onBefore:function( e, elem, \$pane, \$items, pos ){
              e.preventDefault();
              if( this.blur ) this.blur();
            },
            onAfter:function( elem ){
              //'this' is the element being scrolled (\$pane) not jqueryfied
            }
          });
        }
      }
    ";
  drupal_add_css(MAXIMENU_PLUGINS . '/others/longmenu/longmenu.css');
      
  if ($longmenu == 'hover') {
    return $longmenu_hover_scroll_js;
  }
  elseif ($longmenu == 'prev_next') {  
    drupal_add_js(MAXIMENU_PLUGINS . '/styles/tab/jquery.scrollTo.min.js');   
    drupal_add_js(MAXIMENU_PLUGINS . '/styles/tab/jquery.serialScroll.min.js');   
    return $longmenu_prev_next_scroll_js;
  }
}  
