<?php
// $Id$

/**
 * @file
 * Maximenu Effects 
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Admin - hide/show options
 *
 */
function _maximenu_admin_js() {
  
  $add_js = "
    jQuery(document).ready(function($){ 
      $('form#maximenu-admin select.maximenu-style').change(function() {
        var style_wrapper = $(this).parent().parent().parent().attr('id');  
        var style_value = $(this).val(); 

        if(style_value == '') {
          $('form#maximenu-admin fieldset#' + style_wrapper + ' .maximenu-nontab-options').show();          
        }
        else {
          $('form#maximenu-admin fieldset#' + style_wrapper + ' .maximenu-nontab-options').hide();        
        }
        if((style_value != 'roundabout') && (style_value != 'accordion')) {
          $('form#maximenu-admin fieldset#' + style_wrapper + ' .maximenu-other-options').show();          
        }
        else {
          $('form#maximenu-admin fieldset#' + style_wrapper + ' .maximenu-other-options').hide();        
        }        
      });

      $('form#maximenu-admin select.maximenu-output').change(function() {
        var parent_wrapper = $(this).parent().parent().parent().attr('id');  
        var output_value = $(this).val(); 
        if(output_value == 'block') {
          $('form#maximenu-admin fieldset#' + parent_wrapper + ' .maximenu-block-options').show();
          $('form#maximenu-admin fieldset#' + parent_wrapper + ' .maximenu-float-options').hide();          
        }
        if(output_value == 'float') {
          $('form#maximenu-admin fieldset#' + parent_wrapper + ' .maximenu-float-options').show();
          $('form#maximenu-admin fieldset#' + parent_wrapper + ' .maximenu-block-options').hide();
        }     
        if(output_value == 'main_menu') {
          $('form#maximenu-admin fieldset#' + parent_wrapper + ' .maximenu-float-options').hide();
          $('form#maximenu-admin fieldset#' + parent_wrapper + ' .maximenu-block-options').hide();
        } 
      });
      $('form#maximenu-admin select.maximenu-x-origin').change(function() {
        var option_wrapper = $(this).parent().parent().parent().attr('id');  
        var x_origin_value = $(this).val(); 
        //alert(option_wrapper);
        if(x_origin_value == 'middle') {
          $('form#maximenu-admin #' + option_wrapper + ' fieldset.maximenu-non-middle-options').hide();
        }
        else {
          $('form#maximenu-admin #' + option_wrapper + ' fieldset.maximenu-non-middle-options').show();
        }     
      });     
    }); ";
  
  drupal_add_js($add_js, "inline");
} 








