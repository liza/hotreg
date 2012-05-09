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
 * hook_theme() implementation merge
 *
 */
function _maximenu_drop_theme() {
  return array(
    'maximenu_drop' => array(
      'template' => 'plugins/styles/drop/maximenu-drop',
      'variables' => array('links' => array(), 'menu_key' => 1),
    ),
    'maximenu_drop_links' => array(
      'template' => 'plugins/styles/drop/maximenu-drop-links',
      'variables' => array('content' => array(), 'maximenu_name' => NULL, 'skin' => NULL, 'disabled' => 0, 'key' => 0, 'code' => 0, 'count' => 0, 'total' => 1),
    ),
    'maximenu_drop_content' => array(
      'template' => 'plugins/styles/drop/maximenu-drop-content',
      'variables' => array('content' => array(),  'maximenu_name' => NULL, 'key' => 0, 'skin' => NULL),
    ),
  );
}


/**
 * Process variables for maximenu-drop.tpl.php
 *
 */
function template_preprocess_maximenu_drop(&$vars) {

  // sort by weight
  uasort($vars['links']['links'], '_maximenu_sort_by_weight'); 
  
  $links = $vars['links'];
  $vars['skin'] = isset($links['skin']) ? $links['skin']: 'none';  
  // menu name
  $vars['maximenu_name'] = _maximenu_string($links['title']); 
  // disable link when active
  $vars['disabled'] = (isset($links['disabled']) && ($links['disabled'] == 1)) ? 1: 0; 
  
  $vars['code'] = $links['code'];

  $vars['count'] = 0;
  $vars['total'] = count($links['links']);
        
  $vars['theme_hook_suggestions'][] = 'maximenu_drop__' . $vars['menu_id'];
  $vars['theme_hook_suggestions'][] = 'maximenu_drop__' . preg_replace('/-/', '_', $vars['maximenu_name']);
  $vars['theme_hook_suggestions'][] = 'maximenu_drop__' . $links['output'];
  //dsm($vars);
}


/**
 * Process variables for maximenu-drop-links.tpl.php
 *
 */
function template_preprocess_maximenu_drop_links(&$vars) {
    
  $disabled = $vars['disabled'];
  $content  = $vars['content'];

  // user roles and permissions
  $roles = isset($vars['content']['roles']) ? $vars['content']['roles']: 0;
  $vars['permission'] = _maximenu_link_visible($roles);

  // php on title
  if (isset(  $content['php_option']) && ($content['php_option'] == 1)) {
    ob_start();
      $content['link_title'] = eval($content['link_title']);
      //$output = ob_get_contents();
    ob_end_clean(); 
  }
        
  // span id
  $span_id = (!empty($content['id'])) ? ' id="' . _maximenu_string($content['id']) . '"': '';        

  // span title
  $span_title = (!empty($content['description']) && ($content['description_option'] == 'hover')) ? 'title="' . check_plain($content['description']) . '"': ''; 
    
  // link content      
  $link_option = _maximenu_link_option($content);
      
  // a link attributes
  $attributes = _maximenu_link_attrib($content);

  // link is disabled when it's on its page
  $uri = trim(request_uri());
  $qpath =  $_GET['q'];
  $path_relative = isset($content['path']) ? base_path() . $content['path']: '';

  // Drupal doesn't accept # as valid path on field input
  $options = array();       
  $options['query']    = isset($content['path_query']) ? _maximenu_path_query($content['path_query']): '';
  $options['fragment'] = isset($content['path_fragment']) ? $content['path_fragment']: '';                
        
  // disables link when active
  $disable_active_link = ($disabled) ? (($path_relative != $uri) && ($content['path'] != $qpath)) ? 1: 0: 1;
      
  if (!empty($content['path']) && $disable_active_link) {
    $path = ($content['path'] == '<front>') ? '': $content['path'];
    // there are too many things inside this $link_options which a regular l() can't handle
    //$out .= l($content['link_title'], $content['path'], array('attributes' => $attributes));
    $vars['link'] = '<a ' . drupal_attributes($attributes) . ' href="' . url($path, $options) . '">' . $link_option . '</a>';
  }
  elseif (!empty($options['query']) || !empty($options['fragment'])) {
    // base path is not necessary when path is empty but anchor is not or query, 
    $vars['link'] = '<a ' . drupal_attributes($attributes) . ' href="' . url('', $options) . '">' . $link_option . '</a>';
  }      
  else {
    // title with javascript should have div tag
    $script_link = _maximenu_string($content['link_title'], FALSE);
      
    // title contains some block elements 
    $link_tag = ((isset($content['php_option']) && ($content['php_option'] == 1)) || ($script_link == 'Script Link')) ? 'div': 'span';

    // useful when you just want a button for getting the content to show and not actually linking to anything
    $vars['link'] = '<' . $link_tag . $span_id . ' class="' . $attributes['class'] . '" ' . $span_title . '>' . $link_option . '</' . $link_tag . '>';
  }
  $maximenu_name = preg_replace('/-/', '_', $vars['maximenu_name']);              
  $vars['theme_hook_suggestions'][] = 'maximenu_drop_links__' . $maximenu_name;
  $vars['theme_hook_suggestions'][] = 'maximenu_drop_links__' . $maximenu_name . '_' . $vars['key'];
  $vars['theme_hook_suggestions'][] = 'maximenu_drop_links__' . _maximenu_string($content['link_title']);
  //dsm($vars);
}


/**
 * Process variables for maximenu-drop-content.tpl.php
 *
 */
function template_preprocess_maximenu_drop_content(&$vars) {
  $maximenu_name = preg_replace('/-/', '_', $vars['maximenu_name']);                
  $vars['theme_hook_suggestions'][] = 'maximenu_drop_content__' . $vars['skin'];
  $vars['theme_hook_suggestions'][] = 'maximenu_drop_content__' . $maximenu_name;
  $vars['theme_hook_suggestions'][] = 'maximenu_drop_content__' . $maximenu_name . '_' . $vars['key'];
  //dsm($vars);
}
