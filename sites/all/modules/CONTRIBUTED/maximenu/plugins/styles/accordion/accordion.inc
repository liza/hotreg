<?php
/* $Id$ */

/**
 * @file
 * Maximenu Style - Accordion
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * hook_theme() implementation merge
 *
 */
function _maximenu_accordion_theme() {
  return array(
    'maximenu_accordion' => array(
      'template' => 'plugins/styles/accordion/maximenu-accordion',
      'variables' => array('links' => array(), 'menu_key' => 1),
    ),
    'maximenu_accordion_links' => array(
      'template' => 'plugins/styles/accordion/maximenu-accordion-links',
      'variables' => array('content' => array(), 'maximenu_name' => NULL, 'key' => 0),
    ),   
    'maximenu_accordion_content' => array(
      'template' => 'plugins/styles/accordion/maximenu-accordion-content',
      'variables' => array('content' => array(), 'maximenu_name' => NULL, 'key' => 0),
    ),
  );
}


/**
 * Process variables for maximenu_accordion.tpl.php
 *
 */
function template_preprocess_maximenu_accordion(&$vars) {

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
        
  $vars['theme_hook_suggestions'][] = 'maximenu_accordion__' . $vars['menu_key'];
  $vars['theme_hook_suggestions'][] = 'maximenu_accordion__' . preg_replace('/-/', '_', $vars['maximenu_name']);
  $vars['theme_hook_suggestions'][] = 'maximenu_accordion__' . $links['output'];
  //dsm($vars);
}


/**
 * Process variables for maximenu_accordion_links.tpl.php
 *
 */
function template_preprocess_maximenu_accordion_links(&$vars) {
  
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

  // dt style doesn't need any a tag
  $vars['accordion_dt'] = '<dt' . $span_id . ' class="' . $attributes['class'] . '" ' . $span_title . '>' . $link_option . '</dt>';

  $maximenu_name = preg_replace('/-/', '_', $vars['maximenu_name']);
  $vars['theme_hook_suggestions'][] = 'maximenu_accordion_links__' . $maximenu_name;
  $vars['theme_hook_suggestions'][] = 'maximenu_accordion_links__' . $maximenu_name . '_' . $vars['key'];
  $vars['theme_hook_suggestions'][] = 'maximenu_accordion_links__' . _maximenu_string($content['link_title']);
  //dsm($vars);
}


/**
 * Process variables for maximenu_accordion_content.tpl.php
 *
 */
function template_preprocess_maximenu_accordion_content(&$vars) {
  $maximenu_name = preg_replace('/-/', '_', $vars['maximenu_name']);
  $vars['theme_hook_suggestions'][] = 'maximenu_accordion_content__' . $maximenu_name;
  $vars['theme_hook_suggestions'][] = 'maximenu_accordion_content__' . $maximenu_name . '_' . $vars['key'];
  //dsm($vars);
}
