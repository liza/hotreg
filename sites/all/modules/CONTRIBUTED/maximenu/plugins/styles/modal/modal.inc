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
 * hook_theme() implementation merge
 *
 */
function _maximenu_modal_theme() {
  return array(
    'maximenu_modal' => array(
      'template' => 'plugins/styles/modal/maximenu-modal',
      'variables' => array('links' => array(), 'menu_key' => 1),
    ),
    'maximenu_modal_links' => array(
      'template' => 'plugins/styles/modal/maximenu-modal-links',
      'variables' => array('content' => array(), 'maximenu_name' => NULL, 'key' => 0, 'code' => 0, 'count' => 0, 'total' => 1),
    ),    
    'maximenu_modal_content' => array(
      'template' => 'plugins/styles/modal/maximenu-modal-content',
      'variables' => array('links' => array(), 'maximenu_name' => NULL),
    ),    
  );
}


/**
 * Process variables for maximenu-modal.tpl.php
 *
 */
function template_preprocess_maximenu_modal(&$vars) {

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
        
  $vars['theme_hook_suggestions'][] = 'maximenu_modal__' . $vars['menu_key'];
  $vars['theme_hook_suggestions'][] = 'maximenu_modal__' . preg_replace('/-/', '_', $vars['maximenu_name']);
  $vars['theme_hook_suggestions'][] = 'maximenu_modal__' . $links['output'];
  //dsm($vars);
}


/**
 * Process variables for maximenu-modal-links.tpl.php
 *
 */
function template_preprocess_maximenu_modal_links(&$vars) {
  
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

  // link is disabled when it's on its page
  $uri = trim(request_uri());
  $path_relative = base_path() . $content['path'];

  // Drupal doesn't accept # as valid path on field input
  $options = array();
  $options['query']    = isset($content['path_query']) ? $content['path_query']: '';
  $options['fragment'] = isset($content['path_fragment']) ? $content['path_fragment']: '';        
      
  // title with javascript should have div tag
  $script_link = _maximenu_string($content['link_title'], FALSE);
      
  // title contains some block elements 
  if ((isset($content['php_option']) && ($content['php_option'] == 1)) || ($script_link == 'Script Link')) {
    // any link with scripts cannot be processed as modal  
    $vars['link'] = '<div' . $span_id . ' class="' . $attributes['class'] . '" ' . $span_title . '>' . $link_option . '</div>';  
  }
  else {
    // useful when you just want a button for getting the content to show and not actually linking to anything
    $vars['link'] = '<a' . $span_id . ' href="#modal-content-' . $vars['code'] . '-' . $vars['key'] . '" rel="maximenu-modal" class="' . $attributes['class'] . '" ' . $span_title . '>' . $link_option . '</a>';
  }          
  $maximenu_name = preg_replace('/-/', '_', $vars['maximenu_name']);  
  $vars['theme_hook_suggestions'][] = 'maximenu_modal_links__' . $maximenu_name;
  $vars['theme_hook_suggestions'][] = 'maximenu_modal_links__' . $maximenu_name . '_' . $vars['key'];  
  $vars['theme_hook_suggestions'][] = 'maximenu_modal_links__' . _maximenu_string($content['link_title']);
  //dsm($vars);
}


/**
 * Process variables for maximenu-modal-content.tpl.php
 *
 */
function template_preprocess_maximenu_modal_content(&$vars) {
  $links = $vars['links'];
  //dsm($links);
  $vars['theme_hook_suggestions'][] = 'maximenu_modal_content__' . $links['style'];
  //$vars['theme_hook_suggestions'][] = 'maximenu_modal_content__' . $links['skin'];  
  $vars['theme_hook_suggestions'][] = 'maximenu_modal_content__' . preg_replace('/-/', '_', $vars['maximenu_name']);
  //dsm($vars);
}
