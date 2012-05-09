<?php
// $Id$

/**
 * @file
 * Maximenu Render
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */


/**
 * Load all Maximenu blocks
 * 
 * This will process all created Maximenu blocks
 *
 */
function _maximenu_block_load($maximenu_delta = '') {
  global $_maximenu_variable;
  $maximenu = $_maximenu_variable;
  $maximenu_delta = explode('-', $maximenu_delta);
  // ex. $maximenu_delta = 'maximenu-1';
  
  $delta = $maximenu_delta[1];
  
  $out = array();
  // delta always starts with 1
  if (isset($maximenu[$delta]) && ($delta != 0)) {
    if (!empty($maximenu) && ($maximenu[$delta]['output'] == 'block')) {   
      // exclude modal window jquery scripts because these will be loaded on maximenu_load()
      // effects won't load on disabled blocks 
      if (!empty($maximenu[$delta]['region']) && ($maximenu[$delta]['style'] == '')) {
        _maximenu_asset_loader($maximenu[$delta]);
      } 
      elseif (!empty($maximenu[$delta]['region']) && (isset($maximenu[$delta]['style']) && ($maximenu[$delta]['style'] != 'modal'))) {
        _maximenu_asset_loader($maximenu[$delta]);
      }
      
      $out['title']   = check_plain($maximenu[$delta]['title']);
      $out['content'] = _maximenu_links_render($maximenu[$delta], $delta);
    }
  }
  return $out;  
}


/**
 * Load all Maximenu Float
 * 
 * This will process the main menu and floating blocks
 *
 */
function _maximenu_load() {
  global $_maximenu_variable;
  $maximenu = $_maximenu_variable;

  $out = array();  
  if (!empty($maximenu)) {
    foreach ($maximenu as $menu_key => $menu_content) {
      
      $visibility_values = isset($menu_content['visibility']) ? $menu_content['visibility']: array('visibility' => 0, 'pages' => '');
      $visibility = _maximenu_menu_visibility($visibility_values);
      
      // render for non-block only
      if ($visibility && ($menu_content['output'] != 'block')) {
        if (!isset($menu_content['style']) || empty($menu_content['style'])) {
          _maximenu_asset_loader($menu_content);
        }
        elseif (isset($menu_content['style']) && ($menu_content['style'] != 'modal')) {
          _maximenu_asset_loader($menu_content);
        }
        $out[$menu_key]['content'] = _maximenu_links_render($maximenu[$menu_key], $menu_key);             
      }   
      // for modal
      if ($visibility) {  
      
        //$out[$menu_key]['title'] = $menu_content['title'];
        $out[$menu_key]['output'] = $menu_content['output'];
        
        // when to render modal content
        //if (isset($menu_content['style']) && ($menu_content['style'] == 'modal')) {
        if ((isset($menu_content['style']) && ($menu_content['style'] == 'modal')) && (isset($menu_content['region']) && !empty($menu_content['region']))) {
          // if it's output is block and visible in region
          $render_modal_content = 1;
        }
        elseif ((isset($menu_content['style']) && ($menu_content['style'] == 'modal')) && (($menu_content['output'] == 'float') || ( $menu_content['output'] == 'menu'))) {
          // if the output is either float or main menu
          $render_modal_content = 1;
        }
        //elseif ($menu_content['output'] == 'block') {
        elseif ((($menu_content['output'] == 'block') && empty($menu_content['region'])) && (isset($menu_content['modal_content']) && ( $menu_content['modal_content'] == 1))) {
          // force render modal content even if it's not visible in any region
          $render_modal_content = 1;
        }
        else {
          // or do not render modal content
          $render_modal_content = 0;
        }
        
        if ($render_modal_content) {  
        
          //dsm($menu_content);
          _maximenu_asset_loader($menu_content);  
          $maximenu_name = _maximenu_string($menu_content['title']); 
               
          if (empty($out[$menu_key]['modal_windows'])) {
            $out[$menu_key]['modal_windows'] = theme('maximenu_modal_content', array('links' => $menu_content, 'maximenu_name' => $maximenu_name)); 
          }
          else {
            $out[$menu_key]['modal_windows'] .= theme('maximenu_modal_content', array('links' => $menu_content, 'maximenu_name' => $maximenu_name));           
          }
        }
      }
    }
  }
  return $out;  
}


/**
 * Loads styles and js
 *
 */
function _maximenu_asset_loader($menu_content = array()) {
  
  // style loader
  $style = $menu_content['style'];
  include_once MAXIMENU_PLUGINS . '/styles/' . $style . '/' . $style . '.render.php';
  $function = '_maximenu_' . $style . '_render';
  
  $add_js = "jQuery(document).ready(function($){";
  $add_js .= $function($menu_content); 
  $add_js .= "});";
  
  drupal_add_js($add_js, "inline");                                     
}


/**
 * Maximenu links rendering engine
 *
 */
function _maximenu_links_render($links = array(), $menu_id = 1) {
  
  // immediately returns nothing if no links
  if (!isset($links['links'])) return NULL;
  
  $style = $links['style'];
  //dsm($style);
  $content = theme('maximenu_' . $style, array('links' => $links, 'menu_id' => $menu_id));
          
  // access on editing
  if (user_access('administer  Maximenu')) $content .= '<div class="maximenu-edit">' . l(t('Maxedit'), 'admin/structure/maximenu/' . $menu_id . '/edit', array('query' => drupal_get_destination())) . '</div>';    
  
  return theme('maximenu_wrapper', array('links' => $links, 'content' => $content));
}


/**
 * Maximenu content rendering engine
 *
 */
function _maximenu_content_render($content = array()) {
  global $_maximenu_block_classes;
  global $_maximenu_visible_blocks;
    
  $visible_blocks = $_maximenu_visible_blocks;
  //dsm($visible_blocks);  
  $block_classes = !empty($_maximenu_block_classes) ? $_maximenu_block_classes: array();  

  $count = 0;
  uasort($content, '_maximenu_sort_by_weight');  
  $total = count($content);
  $out = '';
  foreach ($content as $key => $prop) {
    $count++;
        
    $module     = $prop['module'];
    $delta      = $prop['delta'];        
    $visibility = (isset($prop['visibility']) && ($prop['visibility'] == 1)) ? in_array($module . '__' . $delta, $visible_blocks) ? 1: 0: 1;

    if ($visibility) {    
      $title               = stripslashes(trim($prop['title'])); // allows markups
      $path                = ($prop['title_path'] == '<front>') ? '': $prop['title_path'];      
      $options             = array();
      $options['query']    = isset($prop['title_path_query']) ? _maximenu_path_query($prop['title_path_query']): '';
      $options['fragment'] = isset($prop['title_path_fragment']) ? $prop['title_path_fragment']: '';        
      $block_title         = (!empty($prop['title_path'])) ? '<a href="' . url($path, $options) . '" title="' . $title . '">' . $title . '</a>': $title;      
      //$block               = module_invoke($module, 'block_view', $delta ); 
      
      // http://drupal.org/node/1422706 - by catch
      // Attached patch takes the output, of the view hook, puts it through block rendering pipeline 
      // (unfortunately there are only private functions for this), then pulls the individual block back out, 
      // this allows those hooks to operate as normal.
      /*
      $blocks = array();
      $blocks[$module . '_' . $delta] = (object) array(
        'delta' => $delta,
        'module' => $module,
        'title' => '<none>', // 'False' doesn't prevent the doubling of the title
        // There is no real region, put a false one in instead.
        'region' => 'maximenu',
      );
      $blocks = _block_render_blocks($blocks);
      $build  = _block_get_renderable_array($blocks);
      // added isset to prevent undefined index
      $block  = isset($build[$module . '_' . $delta]) ? array('content' => $build[$module . '_' . $delta]): '';
      */

      $blocks = array();
      $index = $module . '_' . $delta;
      $blocks[$index] = (object) array(
        'delta' => $delta,
        'module' => $module,
        'title' => '<none>',
        // There is no real region, put a false one in instead.
        'region' => 'maximenu',
      );
      $blocks = _block_render_blocks($blocks);
      $build = _block_get_renderable_array($blocks);
      if (isset($build[$index])) {
        $block = array('content' => $build[$index]);

        $classes = ($count == 1) ? ' first': '';
        if ($count == $total) $classes .= ' last'; 
        $classes .=  isset($block_classes[$module][$delta]) ? ' ' . $block_classes[$module][$delta]: ''; 

        $out .= theme('maximenu_content', array('block' => $block, 'module' => $module, 'delta' => $delta, 'classes' => $classes, 'title' => $title, 'block_title' => $block_title));
      }
      /*
      $classes = ($count == 1) ? ' first': '';
      if ($count == $total) $classes .= ' last'; 
      $classes .=  isset($block_classes[$module][$delta]) ? ' ' . $block_classes[$module][$delta]: ''; 
      
      $out .= theme('maximenu_content', array('block' => $block, 'module' => $module, 'delta' => $delta, 'classes' => $classes, 'title' => $title, 'block_title' => $block_title));                               
      */
    }
  }
  return $out;
} 


/**
 * Preprocess variables for maximenu-wrapper.tpl.php
 *
 */
function template_preprocess_maximenu_wrapper(&$vars) {

  $links = $vars['links'];
  $vars['maximenu_name'] = _maximenu_string($links['title']); 
  $vars['link_code']     = isset($links['code']) ? trim($links['code']): 'u' . $vars['user']->uid . '-' . mt_rand();  

  $vars['theme_hook_suggestions'][] = 'maximenu_wrapper__' . $vars['link_code'];
  $vars['theme_hook_suggestions'][] = 'maximenu_wrapper__' . $vars['maximenu_name'];
  //dsm($vars);
}


/**
 * Preprocess variables for maximenu-content.tpl.php
 *
 */
function template_preprocess_maximenu_content(&$vars) {
  if (!isset($vars['block']['content'])) $vars['block']['content'] = '';   
  $vars['theme_hook_suggestions'][] = 'maximenu_content__' . $vars['module'];
  $vars['theme_hook_suggestions'][] = 'maximenu_content__' . $vars['module'] . '_' . $vars['delta'];
  //dsm($vars);
}



