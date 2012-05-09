<?php
// $Id$

/**
 * @file
 * Maximenu Admin Utilities
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
define('MAXIMENU_PATH', drupal_get_path('module', 'maximenu')); 
define('MAXIMENU_PLUGINS', drupal_get_path('module', 'maximenu') . '/plugins'); 

/**
 * Implementation of hook_init().
 *
 */
function maximenu_init() {
  global $_maximenu_variable;
  global $_maximenu_settings;
  global $_maximenu_block_classes;  
  global $_maximenu_visible_blocks;

  // Maximenu global variable
  $_maximenu_variable = variable_get('maximenu', array()); 
  $_maximenu_settings = variable_get('maximenu_settings', array());
  $_maximenu_visible_blocks = _maximenu_blocks_visibility();  
  
  // add om tools custom block classes    
  if (module_exists('om_tools')) {

    $om_tools_values = variable_get('om_tools', '');
    $blocks = isset($om_tools_values['block']) ? $om_tools_values['block']: array();
    
    if (!empty($blocks) && ($blocks['block_classes_switch'] == 1)) {
      $_maximenu_block_classes = array();
      foreach ($blocks as $block_key => $block_class) {
        $block  = explode('___', $block_key);
        if (isset($block[1])) { // filters switch/reset
          $module = $block[1];
          $delta  = $block[2];
          $_maximenu_block_classes[$module][$delta] = $block_class;
        }
      }
    }
  }  

  /**
   * Includes *.inc files in the plugins folders
   *
   */
  $excluded = array('skins', 'outputs');
  foreach ($_maximenu_settings as $plugin => $plugin_options) {
    if (!in_array($plugin, $excluded)) {
      foreach ($plugin_options as $option => $option_val) {
        $option_file = MAXIMENU_PLUGINS . '/' . $plugin . '/' . $option . '/' . $option . '.inc';
        if (file_exists($option_file)) include_once $option_file;
      }
    }
  }
}

/**
 * Converts string either to a title or a class
 *
 */
function _maximenu_string($string = NULL, $class = TRUE) {
  // for the purpose of just showing the classes and title
  // any tags are stripped off
  $string = trim(strip_tags($string));
  // if there are slashes
  $string = stripslashes($string);  
  // ignores characters other than a-z, A-Z, 0-9
  $string_class = preg_replace('/[^a-zA-Z0-9-]+/', ' ', $string);  
  // allows all characters
  $string_title = $string; 
  // space and underscore are converted depending on use
  $pattern = '/[ _]+/';     
  $string = ($class == FALSE) ? ucwords(preg_replace($pattern, ' ', $string_title)): strtolower(preg_replace($pattern, '-', $string_class));
  // if result is none for non-class
  if (empty($string) && $class == FALSE) $string = 'Script Link';
  
  return $string;
}


/**
 * Maximenu gets options from directories
 * - reads directory names
 */
function _maximenu_options_get($dir = NULL) {
  $files = array();
  $dir = MAXIMENU_PLUGINS . '/' . $dir . '/';
  $folders = scandir($dir);
  $excluded_files = array('.', '..', '.cvs', '.svn', '.git');

  foreach ($folders as $key => $val) {
    if (!in_array($val, $excluded_files)) (is_dir($dir . $val)) ? $files[$val] = _maximenu_string($val, FALSE): ''; 
  }
  return $files;
}


/**
 * Sorting by weight
 *
 */
function _maximenu_sort_by_weight($a, $b) {
  return $a['weight'] - $b['weight'];
}


/**
 * Path queries 
 *
 * On D7 $option['query'] becomes purely an array
 * cannot accept anymore a string
 *
 */
function _maximenu_path_query($path_query = array()) { 
  //dsm($path_query);       
  $out = array();
  $queries = explode('&', $path_query);

  if (!empty($queries)) {
    foreach ($queries as $kquery => $query) {
      if (!empty($query)) {
        $query = explode('=', $query);
        if (!empty($query)) $out[$query[0]] = $query[1];
      }
    }
  }
  return $out;
} 


/**
 * Link Options
 *
 */
function _maximenu_link_option($content = array()) {

  // img hover replacement class
  $icon_option = (isset($content['icon_option']) && ($content['icon_option'] == 1)) ? 'class="hover"': '';

  // title, icon options
  $link_title        = stripslashes($content['link_title']);
  $link_title_option = isset($content['link_title_option']) ? $content['link_title_option']: '';
  $link_icon         = !empty($content['path_icon']) ? '<img ' . $icon_option . ' src="' . url($content['path_icon']) . '" alt="' . _maximenu_string($link_title, FALSE) . '" />': '';
      
  if ($link_title_option == 'title_icon') {
    $link_option = $link_icon . ' ' . $link_title;
  }
  elseif ($link_title_option == 'icon') {
    $link_option = $link_icon;
  }
  else {
    $link_option = $link_title;
  }
  
  // description as subtitle
  $link_subtitle = (!empty($content['description']) && ($content['description_option'] != 'hover')) ? '<span class="subtitle">' . check_plain($content['description']) . '</span>': ''; 
        
  return $link_option . $link_subtitle;
}
  
  
/**
 * Link attribute
 *
 */
function _maximenu_link_attrib($content = array()) {
  // initialize link attributes and classes
  $attributes = array();
      
  // id link attributes
  if (!empty($content['id'])) $attributes['id'] = _maximenu_string($content['id']);

  // user added custom classes
  $custom_classes = '';
  $classes = isset($content['class']) ? explode(' ', $content['class']): '';
  if (!empty($classes)) {
    foreach ($classes as $ckey => $class) {
      $custom_classes .= ' ' . _maximenu_string($class);
    }
  }
  $title_class = _maximenu_string($content['link_title']);
  if (empty($title_class)) $title_class = 'script';
  
  // class link attributes      
  $attributes['class'] = 'link' . $custom_classes . ' link-' . $title_class;  
  
  // active class   
  $uri = trim(request_uri());
  $path_relative = isset($content['path']) ? base_path() . $content['path']: '';  
  $path = isset($content['path']) ? $content['path']: '';  
      
  if (($_GET['q'] == $path) || ($path_relative == $uri)) $attributes['class'] .= ' active'; 
  
  if ((empty($path) || ($path == '<front>')) && drupal_is_front_page()) $attributes['class'] .= ' active';   
  
  // link description options
  if (!empty($content['description']) && ($content['description_option'] == 'hover')) $attributes['title'] = check_plain($content['description']); 

  // relationship
  if (!empty($content['rel'])) $attributes['rel'] = $content['rel'];

  // target
  if (!empty($content['target']) && ($content['target'] != '_self')) $attributes['target'] = $content['target'];

  return $attributes;
} 


/**
 * Classes aggregator
 *
 */
function _maximenu_classes($links = array()) {
  $classes = array();
  
  // bubble, simple, no_style  
  $classes[] = isset($links['skin']) ? $links['skin']: '';

  // style
  if (isset($links['style']) && !empty($links['style'])) {
    if (($links['style'] == 'scrollh') || ($links['style'] == 'scrollv')) {
      $classes[] = 'tab-' . $links['style'];    
    }
    else {
      $classes[] = 'style-' . $links['style'];
    }
  } 
    
  // block, float, main_menu  
  $classes[] = $links['output'];
  
  if ($links['output'] == 'block') {
    // row, column
    $classes[] = isset($links['stacking']) ? $links['stacking']: 'row';   
    // up, right, down, left
    $classes[] = isset($links['direction']) ? $links['direction']: 'block-down';   
  }
  elseif ($links['output'] == 'float') {
    // left, middle, right
    $classes[] = isset($links['orientation']) ? $links['orientation']: 'horizontal';       
    $classes[] = isset($links['stacking']) ? $links['stacking']: 'row';   
    $classes[] = isset($links['x_origin']) ? $links['x_origin']: 'left';   
    $classes[] = isset($links['y_origin']) ? $links['y_origin']: 'bottom';   
  }
  elseif ($links['output'] == 'main_menu') {
    // no class
  }  
  
  // fisheye dock class
  if (isset($links['dock'])) $classes[] = 'dock';
  
  $out = '';
  foreach ($classes as $key => $class) {
    $out .= ' maximenu-' . _maximenu_string($class);   
  }
  return $out;
}   


/**
 * Maximenu inline styling
 *
 */
function _maximenu_inline_style($links = array()) {
  $out = '';
  if ($links['output'] == 'block') {
    // do nothing now, maybe in the future
  }  
  elseif ($links['output'] == 'float') {
    // fixed/absolute
    $position    = (isset($links['float_options']['position']) && ($links['float_options']['position'] == 'absolute')) ? 'absolute': 'fixed';   
    // left/middle/right
    if (isset($links['float_options']['x_origin']) && ($links['float_options']['x_origin'] != 'middle')) {
      $x_origin    = isset($links['float_options']['x_origin']) ? $links['float_options']['x_origin']: 'left';     
      $x_value     = isset($links['float_options']['x_value']) ? $links['float_options']['x_value']: 10;
      $x_px        = ($x_value != 0) ? 'px': '';
      $x_prop      = $x_origin . ': ' . $x_value . $x_px . '; '; 
    } 
    else {
      $x_prop = ''; // jquery will determine the position
    }  
    // top/bottom
    $y_origin    = isset($links['float_options']['y_origin']) ? $links['float_options']['y_origin']: 'bottom';     
    $y_value     = isset($links['float_options']['y_value']) ? $links['float_options']['y_value']: 10; 
    $y_px        = ($y_value != 0) ? 'px': '';    
    $y_prop      = $y_origin . ': ' . $y_value . $y_px . '; ';
               
    $out = ' style="position: ' . $position . '; ' . $x_prop . $y_prop . '" ';   
  }
  elseif ($links['output'] == 'main_menu') {
    // do nothing now, maybe in the future
  }
  
  return $out;
}


/**
 * Link classes processor
 *
 */
function _maximenu_link_classes($content = array(), $permission = array(), $count = 0, $total = 0) {

  // initialize link classes
  $classes = array();     
                        
  // standard li classes
  if ($count == 1) $classes[] = 'first';
  if ($count == $total) $classes[] = 'last';  
      
  // link title class
  $classes[] = 'leaf-' . $content['link_title'];     

  // active class   
  $path = isset($content['path']) ? $content['path']: '';    
  if ($_GET['q'] == $path) $classes[] = 'active'; 
      
  if ((empty($path) || ($path == '<front>')) && drupal_is_front_page()) $classes[] = 'active';   
      
  // user role class
  if (is_array($permission)) {
    foreach ($permission as $pkey => $pval) {
      $classes[] = 'leaf-role-' . $pval; 
    }  
  }      
        
  // title, icon options
  $link_title_option = isset($content['link_title_option']) ? $content['link_title_option']: '';
  if ($link_title_option == 'title_icon') {
    $classes[] = 'leaf-title-icon';
  }
  elseif ($link_title_option == 'icon') {
    $classes[] = 'leaf-icon';
  }
  
  $out = 'leaf';
  foreach ($classes as $key => $class) {
    $out .= ' ' . _maximenu_string($class);   
  }
  return $out;
}


/**
 * Link visibility per user role
 *
 */
function _maximenu_link_visible($roles = array()) {
  global $user;

  // user roles  
  $user_id = $user->uid;  
  $user_roles = $user->roles;  
  // user permission to view the link
  // roles
  // 1 - anonymous
  // 2 - authenticated

  $permission = array();
  $anyone = 0;
  if (!empty($roles) && ($roles != 0)) {
    foreach ($roles as $role => $value) {
     // check roles
     if (($value != 0) && isset($user_roles[$role])) $permission[$role] = $user_roles[$role];
     // for all roles are unchecked
     if ($value != 0) $anyone++;       
    }
  }
  // the link will be visible if the user has permission 
  if (!empty($permission)) {
    return $permission;
  }
  elseif ($anyone == 0) { // the links is for everybody
    return 1;
  }
  //elseif ($user_id == 1) { // the user is a super user
    //return 1;
  //}
  else {
    return 0;
  }
}      


/**
 * Maximenu content block visibility check
 *
 */
function _maximenu_blocks_visibility() {
  global $user, $theme_key;
  
  $visible_blocks = array();
  
  $query = db_select('block', 'b');
  $result = $query
    ->fields('b')
    ->condition('b.theme', $theme_key)
    //->condition('b.status', 1)
    ->orderBy('b.region')
    ->orderBy('b.weight')
    ->orderBy('b.module')
    ->addTag('block_load')
    ->addTag('translatable')
    ->execute();

  $block_info = $result->fetchAllAssoc('bid');
  // Allow modules to modify the block list.
  drupal_alter('block_list', $block_info);

  $blocks = array();
  foreach ($block_info as $block) {
    $blocks[$block->module . '__' . $block->delta] = $block;
  }

  // Build an array of roles for each block.
  $block_roles = array();
  $result = db_query('SELECT module, delta, rid FROM {block_role}');
  foreach ($result as $record) {
    $block_roles[$record->module][$record->delta][] = $record->rid;
  }

  foreach ($blocks as $key => $block) {
    //if (!isset($block->theme) || !isset($block->status) || $block->theme != $theme_key || $block->status != 1) {
      // This block was added by a contrib module, leave it in the list.
      //continue;
    //}

    // If a block has no roles associated, it is displayed for every role.
    // For blocks with roles associated, if none of the user's roles matches
    // the settings from this block, remove it from the block list.
    if (isset($block_roles[$block->module][$block->delta]) && !array_intersect($block_roles[$block->module][$block->delta], array_keys($user->roles))) {
      // No match.
      unset($blocks[$key]);
      continue;
    }

    // Use the user's block visibility setting, if necessary.
    if ($block->custom != BLOCK_CUSTOM_FIXED) {
      if ($user->uid && isset($user->data['block'][$block->module][$block->delta])) {
        $enabled = $user->data['block'][$block->module][$block->delta];
      }
      else {
        $enabled = ($block->custom == BLOCK_CUSTOM_ENABLED);
      }
    }
    else {
      $enabled = TRUE;
    }

    // Limited visibility blocks must list at least one page.
    if ($block->visibility == BLOCK_VISIBILITY_LISTED && empty($block->pages)) {
      $enabled = FALSE;
    }

    if (!$enabled) {
      unset($blocks[$key]);
      continue;
    }

    // Match path if necessary.
    if ($block->pages) {
      // Convert path to lowercase. This allows comparison of the same path
      // with different case. Ex: /Page, /page, /PAGE.
      $pages = drupal_strtolower($block->pages);
      if ($block->visibility < BLOCK_VISIBILITY_PHP) {
        // Convert the Drupal path to lowercase
        $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));
        // Compare the lowercase internal and lowercase path alias (if any).
        $page_match = drupal_match_path($path, $pages);
        if ($path != $_GET['q']) {
          $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
        }
        // When $block->visibility has a value of 0 (BLOCK_VISIBILITY_NOTLISTED),
        // the block is displayed on all pages except those listed in $block->pages.
        // When set to 1 (BLOCK_VISIBILITY_LISTED), it is displayed only on those
        // pages listed in $block->pages.
        $page_match = !($block->visibility xor $page_match);
      }
      elseif (module_exists('php')) {
        $page_match = php_eval($block->pages);
      }
      else {
        $page_match = FALSE;
      }
    }
    else {
      $page_match = TRUE;
    }

    if (!$page_match) {
      unset($blocks[$key]);
    }
    if ($page_match) $visible_blocks[] = $key;    
  }  
  //dsm($visible_blocks);
  return $visible_blocks;
}


/**
 * Block edit link
 *
 */
function _maximenu_block_edit($module, $delta, $title = NULL) {
  $vars = array();
  // Display 'edit block' for custom blocks.
  if ($module == 'block') {
    $options = array(
      'attributes' => array(
        'title' => t('Edit this block\'s content.'),
        'class' => array('block-edit'),
      ),
      'query' => drupal_get_destination(),
      'html' => TRUE,
    );     
    $vars[] = l( t('edit block'), 'admin/structure/block/manage/' . $module . '/' . $delta . '/configure', $options);
  }
  // Display 'configure' for other blocks.
  else {
    $options = array(
      'attributes' => array(
        'title' => t('Configure ' . $title),
        'class' =>  array('block-config'),
      ),
      'query' => drupal_get_destination(),
      'html' => TRUE,
    );    
    $vars[] = l( t('configure'), 'admin/structure/block/manage/' . $module . '/' . $delta . '/configure', $options);
  }
  // Display 'edit view' for Views blocks.
  if ($module == 'views' && user_access('administer views')) {
    //list($view_name, $view_block) = explode('-block', $delta);
    $view = explode('-block', $delta);   
    $view_name = isset($view[0]) ? $view[0]: ''; 
    $view_block = isset($view[1]) ? $view[1]: ''; 
    
    $options = array(
      'attributes' => array(
        'title' => t('Edit this view.'),
        'class' =>  array('block-edit-view'),
      ),
      'query' => drupal_get_destination(),
      'html' => TRUE,
    );
    $vars[] = l( t('edit view'), 'admin/structure/views/view/' . $view_name . '/edit/block' . $view_block, $options);
  }
  // Display 'edit menu' for Menu blocks.
  elseif ((($module == 'menu') || ($module == 'system')) && user_access('administer menu')) {
    $menu_name = $delta;
    $options = array(
      'attributes' => array(
        'title' => t('Edit this menu.'),
        'class' =>  array('block-edit-menu'),
      ),
      'query' => drupal_get_destination(),
      'html' => TRUE,
    );
    $vars[] = l( t('edit menu'), 'admin/structure/menu/manage/' . $menu_name, $options);
  }

  $out = '<div class="edit-block">' . implode(' ', $vars) . '</div>';
  return $out;
}


/**
 * Menu Visibility
 *
 */
function _maximenu_menu_visibility($menu_visibility = FALSE) {
  if ($menu_visibility['pages']) {
    if ($menu_visibility['visibility'] < 2) {
      $path = drupal_get_path_alias($_GET['q']);
      $page_match = drupal_match_path($path, $menu_visibility['pages']);
      if ($path != $_GET['q']) {
        $page_match = $page_match || drupal_match_path($_GET['q'], $menu_visibility['pages']);
      }
      $page_match = !($menu_visibility['visibility'] xor $page_match);
    }
    elseif (module_exists('php')) {
        $page_match = php_eval($menu_visibility['pages']);
    }
    else {
      $page_match = FALSE;
    }
  }
  else {
      $page_match = TRUE;
  }
  return $page_match;
}


/**
 * Block Configure Page - Get delta visiblity value form _maximenu_variable;
 *
 */
function _maximenu_form_alter(&$form, $form_state, $form_id) { 
  switch ($form_id) {
    case 'block_admin_configure':
      if ($form['module']['#value'] == 'maximenu') $form['#submit'][] = '_maximenu_block_admin_configure_submit_handler';
      break;
    case 'block_admin_display_form':
      $form['#submit'][] = '_maximenu_block_admin_display_submit_handler';
      break;
  }
}


/**
 * Updates Maximenu variable on saving block configuration
 *
 */
function _maximenu_block_admin_configure_submit_handler(&$form, $form_state) {
  $delta = $form_state['values']['delta'];
  $delta = explode('-', $delta);
  
  if ($delta[1] == 'maximenu') {
    global $_maximenu_variable;
    $maximenu = $_maximenu_variable;
    $delta = $delta[2];
    $maximenu[$delta]['visibility'] = $form_state['values']['visibility'];   
    $maximenu[$delta]['pages'] = $form_state['values']['pages'];
    variable_set('maximenu', $maximenu);
  }
}


/**
 * Updates Maximenu variable on saving block display
 *
 */
function _maximenu_block_admin_display_submit_handler(&$form, $form_state) {
  global $_maximenu_variable;
  $maximenu = $_maximenu_variable;
  $blocks = $form_state['values']['blocks'];
  
  $count = 0;
  foreach ($blocks as $module_delta => $values) {
    if ($values['module'] == 'maximenu') {
      $delta = explode('-', $values['delta']);
      $delta = $delta[2];
      $maximenu[$delta]['region'] = $values['region'];     
      $maximenu[$delta]['weight'] = $values['weight']; 
      $count++;           
    }
  }
  // will only process if Maximenu blocks are present
  if ($count != 0) variable_set('maximenu', $maximenu);  
}


/**
 * Block db updater
 *
 */
function _maximenu_update_blocks_db($menu_id = 0, $maximenu_content = array()) {
  if ($menu_id != 0) {
    if ((isset($maximenu_content['delete']) && ( $maximenu_content['delete'] == 1)) || ($maximenu_content['output'] != 'block')) {
      // menu as float must be deleted from block table
      db_delete('block')->condition('module', 'maximenu')->condition('delta', 'maximenu-' . $menu_id)->execute();            
    }
    elseif (($maximenu_content['output'] == 'block') && isset($maximenu_content['output_options']['region'])) {
    
      $theme_default = variable_get('theme_default', 'garland');

      $existing = db_select('block', 'n')
        ->fields('n')
        ->condition('module', 'maximenu', '=')
        ->condition('delta', 'maximenu-' . $menu_id, '=') 
        ->condition('theme', $theme_default, '=')                           
        ->execute()
        ->fetchObject(); 

      if (!empty($existing->delta)) {                      
        // if the menu is existing   
        $status = (!empty($maximenu_content['output_options']['region'])) ? 1: 0;
        
        db_update('block')
          ->fields(array(
            'status'     => $status,
            'weight'     => $maximenu_content['output_options']['weight'],                                  
            'region'     => $maximenu_content['output_options']['region'],  
            'visibility' => (int) $maximenu_content['output_options']['visibility'],
            'pages'      => trim($maximenu_content['output_options']['pages']),
          ))
          ->condition('module', 'maximenu')
          ->condition('delta', 'maximenu-' . $menu_id) 
          ->condition('theme', $theme_default)                             
          ->execute();
      }
      else {  
        // because we are handling the visibility, etc., we have to handle everything to synch with blocks settings
        $status = (!empty($maximenu_content['region'])) ? 1: 0;

        db_insert('block')
          ->fields(array(
            'module'     => 'maximenu', 
            'delta'      => 'maximenu-' . $menu_id,                                     
            'theme'      => $theme_default,   
            'status'     => $status,                   
            'weight'     => $maximenu_content['output_options']['weight'],                                 
            'region'     => $maximenu_content['output_options']['region'],  
            'visibility' => (int) $maximenu_content['output_options']['visibility'],
            'pages'      => trim($maximenu_content['output_options']['pages']),
            'cache'      => -1,  // no cache
          ))->execute();
      }                     
    }
  }
}

          
/**
 * Get the hierarchy of menus
 *
 */
function _menu_tree_options(&$options, $menus, $menu_name = 'navigation') {
  global $_maximenu_existing_menus;
  $existing = $_maximenu_existing_menus[$menu_name];
        
  static $level = 1; // count level for indents on divs on html file
  $tab = '-';        // init tab for indents on divs on html file
  // for source formating
  // calculates number of tabs
  for ($i = 1; $i < $level; $i++) {
    $tab .= '-';
  }    
  foreach ($menus as $mlid => $link_title) {
    if (isset($existing[$mlid])) {
      $level++; 
      $options[$mlid] = $tab . $link_title;
      _menu_tree_options($options, $existing[$mlid], $menu_name);
      $level--;       
    }
    else {
      $options[$mlid] = $tab . $link_title;
    }
  }
}
