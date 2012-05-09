<?php
/* $Id$ */

/**
 * @file
 * Maximenu Style - Roundabout
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * hook_theme() implementation merge
 *
 */
function _maximenu_roundabout_theme() {
  return array(
    'maximenu_roundabout' => array(
      'template' => 'plugins/styles/roundabout/maximenu-roundabout',
      'variables' => array('links' => array(), 'menu_key' => 1),
    ),
    'maximenu_roundabout_links' => array(
      'template' => 'plugins/styles/roundabout/maximenu-roundabout-links',
      'variables' => array('content' => array(), 'maximenu_name' => NULL, 'key' => 0, 'code' => 0, 'count' => 0, 'total' => 1),
    ), 
    'maximenu_roundabout_content' => array(
      'template' => 'plugins/styles/roundabout/maximenu-roundabout-content',
      'variables' => array('content' => array(), 'maximenu_name' => NULL, 'key' => 0),
    ),
  );
}


/**
 * Process variables for maximenu_roundabout.tpl.php
 *
 */
function template_preprocess_maximenu_roundabout(&$vars) {

  // sort by weight
  uasort($vars['links']['links'], '_maximenu_sort_by_weight'); 
  
  $links = $vars['links'];
  
  // menu name
  $vars['maximenu_name'] = _maximenu_string($links['title']); 

  // disable link when active
  $vars['disabled'] = (isset($links['disabled']) && ($links['disabled'] == 1)) ? 1: 0; 
  
  $vars['code'] = $links['code'];

  $vars['count'] = 0;
  $vars['total'] = count($links['links']);
        
  $vars['theme_hook_suggestions'][] = 'maximenu_roundabout__' . $vars['menu_key'];
  $vars['theme_hook_suggestions'][] = 'maximenu_roundabout__' . preg_replace('/-/', '_', $vars['maximenu_name']);
  $vars['theme_hook_suggestions'][] = 'maximenu_roundabout__' . $links['output'];
  //dsm($vars);
}


/**
 * Process variables for om_maximenu_roundabout_links.tpl.php
 *
 */
function template_preprocess_maximenu_roundabout_links(&$vars) {
    
  $content = $vars['content'];
  
    // user roles and permissions
  $vars['permission'] = _maximenu_link_visible($vars['content']['roles']);

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

  // title with javascript should have div tag
  $script_link = _maximenu_string($content['link_title'], FALSE);
      
  // title contains some block elements 
  $link_tag = ((isset($content['php_option']) && ($content['php_option'] == 1)) || ($script_link == 'Script Link')) ? 'div': 'span';

  // useful when you just want a button for getting the content to show and not actually linking to anything
  $vars['link'] = '<' . $link_tag . $span_id . ' class="' . $attributes['class'] . '" ' . $span_title . '>' . $link_option . '</' . $link_tag . '>';

  $maximenu_name = preg_replace('/-/', '_', $vars['maximenu_name']);              
  $vars['theme_hook_suggestions'][] = 'maximenu_roundabout-links__' . $maximenu_name;
  $vars['theme_hook_suggestions'][] = 'maximenu_roundabout-links__' . $maximenu_name . '_' . $vars['key'];
  $vars['theme_hook_suggestions'][] = 'maximenu_roundabout-links__' . _maximenu_string($content['link_title']);
  //dsm($vars);
}


/**
 * Process variables for maximenu_roundabout_content.tpl.php
 *
 */
function template_preprocess_maximenu_roundabout_content(&$vars) {
  $maximenu_name = preg_replace('/-/', '_', $vars['maximenu_name']);
  $vars['theme_hook_suggestions'][] = 'maximenu_roundabout_content__' . $maximenu_name;
  $vars['theme_hook_suggestions'][] = 'maximenu_roundabout_content__' . $maximenu_name . '_' . $vars['key'];
  //dsm($vars);
}
