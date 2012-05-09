<?php
// $Id$

/**
 * @file
 * Maximenu Admin Configuration
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Admin Form: UI
 *
 */
function maximenu_admin($form, &$form_state, $op = NULL) {

  $active_tab = variable_get('maximenu_active_tab', '');
  //dsm($form_state);
  /**
   * Meximenu uses a single container for all admin forms,
   * this variable will get the right form (list/add/edit/delete) which depends on the $op variable
   */
  $maximenu_form = _maximenu_admin($op);
  
  $form_new = array();
  
  /**
   * Maximenu breadcrumb
   * - for easier menu, links and attached blocks navigation
   */ 
  if (($op != '') && ($op != 'list') && ($op != 'delete')) {
    $form_new['back'] = array(
      '#type'   => 'markup',
      '#markup' => l(t('Maximenu'), 'admin/structure/maximenu', array('attributes' => array('title' => t('Go back to List of All Menus')))) . ' » ' . drupal_get_title(),
    );    
  }
  // List/Add/Edit/Delete
  $form_new['maximenu'] = array(
    //'#type'  => 'fieldset',
    '#type'  => 'vertical_tabs',
    '#default_tab' => $active_tab,
    '#tree'   => TRUE,
  );    
  $form_new['maximenu'] += $maximenu_form;
  
  if (($op != 'list') && ($op != NULL)) {  
    // show buttons on editing
    if ($op == 'delete') {
      $form_new['delete'] = array( '#type' => 'submit', '#value' => t('Delete'), );
    }
    else {
      $form_new['save'] = array( '#type' => 'submit', '#value' => t('Save'), );
    }  
    $form_new['cancel'] = array( '#type' => 'submit', '#value' => t('Cancel'), ); 
  }
  //dsm($form);
  return $form_new;  
}


/**
 * Admin Form: Logic for getting the forms
 *
 */
function _maximenu_admin($op = NULL) {
  global $_maximenu_variable;  

  $maximenu       = $_maximenu_variable;
  $prefix         = '';
  $suffix         = '';
  $maximenu_title = '';
  $form_new  = array();

  //unset($maximenu['maximenu__active_tab']);  
  // always sort menu by keys      
  if (!empty($maximenu)) ksort($maximenu);
 
  if (($op == 'list') || ($op == NULL)) { // default page
    // Title
    $suffix         = t('of All Menus');
    $maximenu_title = t('List');
    // Form
    $form_new = _maximenu_list();  
  } 
  else {     
    // get the 3rd argument in /admin/structure/maximenu/1/edit
    // this serves as the menu ID        
    $menu_id = arg(3); 
    //dsm(arg());
    // Show editing forms
    if ($op == 'add') {
      // Title
      $prefix         = t('Add');
      $maximenu_title = t('New Menu');
      // Form  
      $form_new += _maximenu_add($maximenu);
    }
    elseif ($op == 'delete') {  
      // Title
      $maximenu_title = t('Are you sure you want to delete @menu?', array('@menu' => $maximenu[$menu_id]['title'])); 

      // Form
      $form_new += _maximenu_delete($menu_id, $maximenu[$menu_id]);
    }  
    elseif ($op == 'clone') {   
      // Edit - Single menu 
      $this_menu = array();
      
      // check if this menu exists
      $this_menu[$menu_id] = isset($maximenu[$menu_id]) ? $maximenu[$menu_id]: drupal_goto('admin/structure/maximenu');
      // assign only a single menu to the variable
      $maximenu = $this_menu;
      // Title
      $prefix         = t('Clone');
      $maximenu_title = $maximenu[$menu_id]['title'] . ' ' . t('(Clone)');   
      // Form
      $form_new += _maximenu_clone($maximenu);
    }     
    elseif ($op == 'edit') {   
      // Edit - Single menu 
      $this_menu = array();
      
      // check if this menu exists
      $this_menu[$menu_id] = isset($maximenu[$menu_id]) ? $maximenu[$menu_id]: drupal_goto('admin/structure/maximenu');
      // assign only a single menu to the variable
      $maximenu = $this_menu;
      // Title
      $prefix         = t('Edit');
      $maximenu_title = $maximenu[$menu_id]['title'];   
      // Form
      $form_new += _maximenu_edit($maximenu);
    } 
    elseif ($op == 'max') {
      // Maxedit - Multiple edit, delete and add another menu   
      // Title
      $suffix         = t('(Add/Edit/Delete All Menus)');
      $maximenu_title = t('Maxedit');  
      // Form
      $form_new += _maximenu_edit($maximenu);
      $form_new += _maximenu_add($maximenu);
    }    
  }
  // set page title
  drupal_set_title($prefix . ' ' . $maximenu_title . ' ' . $suffix);
  
  return $form_new;
}  

/**
 * Admin Form: List
 *
 */
function _maximenu_list() {
  global $_maximenu_variable;  
  global $_maximenu_settings;
  $settings = $_maximenu_settings;
  //dsm($settings);
  $maximenu = $_maximenu_variable;
  
  $style_options = isset($settings['styles']) ? $settings['styles']: '';
                
  $rows = array();
  if (!empty($maximenu)) {      
    foreach ($maximenu as $menu_id => $menu_content) {
      $links = isset($menu_content['links']) ? $menu_content['links']: 0;
      $row = array();
      $row[] = $menu_content['title']; 
      $row[] = isset($style_options[$menu_content['style']]) ? $style_options[$menu_content['style']]: '';         
      $row[] = ucwords($menu_content['output']); 
      $row[] = l(count($links), 'admin/structure/maximenu/' . $menu_id . '/links', array('attributes' => array('title' => t('Edit links.'))));                       
      $row[] = l(t('Edit'), 'admin/structure/maximenu/' . $menu_id . '/edit', array('attributes' => array('title' => t('Edit this menu.'))));
      $row[] = l(t('Clone'), 'admin/structure/maximenu/' . $menu_id . '/clone', array('attributes' => array('title' => t('Clone this menu.'))));
      $row[] = l(t('Delete'), 'admin/structure/maximenu/' . $menu_id . '/delete', array('attributes' => array('title' => t('Delete this menu.'))));
      $rows[] = array('data' => $row);
    }
  }  
  $rows[] = array(
    'data' => array(
      'data' => l(t('Add Menu'), 'admin/structure/maximenu/add', array('attributes' => array('title' => t('Add another menu.')))),
      'attributes' => array('colspan' => 6),
    ), 
  );

  $header = array(t('Menu'), t('Style'), t('Output'), t('Links'), t('Details'), '', '');
 
  $menus = theme('table', array('header' => $header, 'rows' => $rows));
  
  $maximenu_array = array();
  $maximenu_array[0] = array(
    '#type'   => 'markup',
    '#markup' => $menus,
  );        
  return $maximenu_array;
}


/**
 * Admin Form: Add
 *
 */
function _maximenu_add($maximenu = array()) {
  $maximenu_array = array();
  // New menu and links
  // new ids relies on highest menu key id
  // adding only 1 new blank menu
  $count = !empty($maximenu) ? max(array_keys($maximenu)) + 1: 1;

  $menu_content_new = array(
    'title'    => 'New Menu',
    'style'    => 'drop', 
    'output'   => 'block',        
    'links'    => array(),
    'menu_id' => $count,
  );   
  $maximenu_array[$count] = array(
    '#type'        => 'fieldset',
    '#title'       => 'New Menu',
    '#collapsible' => TRUE,
    '#collapsed'   => (arg(3) == 'maxedit') ? TRUE: FALSE,
    '#attributes' => array('id' => 'maximenu-fieldset-new-menu', 'class' => array('maximenu-fieldset')),                
    '#description' => t('Change "New Menu" title to activate this menu.'),
  );  
  $maximenu_array[$count] += _maximenu_edit_details($menu_content_new);
  
  return $maximenu_array;
}


/**
 * Admin Form: Clone
 *
 */
function _maximenu_clone($maximenu = array()) {
  global $user;
  $maximenu_array = array();
  
  if (!empty($maximenu)) {
    foreach ($maximenu as $menu_id => $menu_content) {
      // setting menu id, code, title to new
      $menu_id_new             = max(array_keys($maximenu)) + 1;
      $menu_content['menu_id_cloned'] = $menu_id; // preserve menu id for the links
      $menu_content['menu_id'] = $menu_id_new;
      $menu_content['code']    = 'u' . $user->uid . '-' . mt_rand();  
      $menu_content['title']   = $menu_content['title'] . ' ' . t('(Clone)'); 

      // counting active links
      $active_links = isset($menu_content['links']) ? count($menu_content['links']): 0;
      //$edit_links = l(t('Edit links'), 'admin/structure/maximenu/' . $menu_id . '/links', array('attributes' => array('title' => t('Edit ' . $menu_content['title'] . ' links'))));  
      
      $maximenu_array[$menu_id_new] = array(
        '#type'        => 'fieldset',
        //'#default_tab' => ($menu_id == 1) ? TRUE: FALSE,
        '#title'       => $menu_content['title'],
        '#collapsible' => TRUE,
        '#collapsed'   => (arg(3) == 'maxedit') ? TRUE: FALSE,
        '#attributes' => array('id' => 'maximenu-fieldset-' . _maximenu_string($menu_content['title']), 'class' => array('maximenu-fieldset')),                
        '#description' => t('Active Link/s:') . ' (' . $active_links . ')', // | ' . $edit_links,
      );  
      //dsm($menu_content);
      //$menu_content['roles']      = $roles;
      $maximenu_array[$menu_id_new] += _maximenu_edit_details($menu_content);
    }
  }
  return $maximenu_array;
} 


/**
 * Admin Form: Edit
 *
 */
function _maximenu_edit($maximenu = array()) {
  $maximenu_array = array();
  
  if (!empty($maximenu)) {
    foreach ($maximenu as $menu_id => $menu_content) {
      // counting active links
      $active_links = isset($menu_content['links']) ? count($menu_content['links']): 0;
      $edit_links = l(t('Edit links'), 'admin/structure/maximenu/' . $menu_id . '/links', array('attributes' => array('title' => t('Edit ' . $menu_content['title'] . ' links'))));  
      
      $maximenu_array[$menu_id] = array(
        '#type'        => 'fieldset',
        //'#default_tab' => ($menu_id == 1) ? TRUE: FALSE,
        '#title'       => $menu_content['title'],
        '#collapsible' => TRUE,
        '#collapsed'   => (arg(3) == 'maxedit') ? TRUE: FALSE,
        '#attributes' => array('id' => 'maximenu-fieldset-' . _maximenu_string($menu_content['title']), 'class' => array('maximenu-fieldset')),                
        '#description' => t('Active Link/s:') . ' (' . $active_links . ') | ' . $edit_links,
      );  
      //dsm($menu_content);
      $menu_content['menu_id']   = $menu_id;
      //$menu_content['roles']      = $roles;    
      $maximenu_array[$menu_id] += _maximenu_edit_details($menu_content);
    }
  }
  return $maximenu_array;
} 
 
/**
 * Admin Form: Settings
 *
 */
function _maximenu_edit_details($menu_content = array()) {
  global $user;
  global $_maximenu_settings;
  $settings = $_maximenu_settings;

  $code          = 'u' . $user->uid . '-' . mt_rand();  
  $menu_id       = $menu_content['menu_id'];
  
  $form_new = array();

  $form_new['code'] = array(
    '#type'  => 'hidden',
    '#default_value' => isset($menu_content['code']) ? trim($menu_content['code']): $code,
  );  

  $form_new['menu_id_cloned'] = array(
    '#type'  => 'hidden',
    '#default_value' => isset($menu_content['menu_id_cloned']) ? trim($menu_content['menu_id_cloned']): 0,
  );
    
  // delete field is for saved menus only
  if (trim($menu_content['title']) != 'New Menu') {
    $form_new['delete'] = array(
      '#type'  => 'checkbox',
      '#title' => t('Delete this menu.'),    
      '#default_value' => 0,
    );
  }  
  $form_new['title'] = array(
    '#type'  => 'textfield',
    '#title' => t('Menu Title'),  
    '#required' => TRUE,  
    '#default_value' => isset($menu_content['title']) ? stripslashes(trim($menu_content['title'])): 'New Menu',
  );  
  
  // Style options
  $style_options = isset($settings['styles']) ? $settings['styles']: array('drop' => 'Drop');
  if (!empty($style_options)) {
    $form_new['style'] = array(
      '#type'  => 'select',
      '#title' => t('Style'),
      '#attributes' => array('class' => array('maximenu-style')),
      '#default_value' => isset($menu_content['style']) ? $menu_content['style']: 'drop',
      '#options' => $style_options,
      '#description' => t('Click "Choose" if you have changed the style, to show style options.'),
    );
    $form_new['style_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Choose'),
      '#attributes' => array('class' => array('next-button')),
      '#submit' => array('maximenu_admin_submit'),
    );
    $form_new['style_options'] = array(
      '#type' => 'fieldset',
      '#title' => 'Options',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );    
    // add style options
    $style = $menu_content['style'];
    include_once MAXIMENU_PLUGINS . '/styles/' . $style . '/' . $style . '.options.php';
    $style_function = '_maximenu_' . $style . '_options';
    $form_new['style_options'] += $style_function($menu_content, $settings); 
  }

  // Output options
  $output_options = isset($settings['outputs']) ? $settings['outputs']: array('block' => 'Block');
  if (!empty($output_options)) {
    $form_new['output'] = array(
      '#type'  => 'select',
      '#title' => t('Output Options'),
      '#options' => $output_options,    
      '#default_value' => isset($menu_content['output']) ? $menu_content['output']: 'block',
      '#attributes' => array('class' => array('maximenu-output')),        
      '#description' => t('Click "Choose" if you have changed the output, to show output options.'),
    );
    $form_new['output_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Choose'),
      '#attributes' => array('class' => array('next-button')),
      '#submit' => array('maximenu_admin_submit'),
    );
    $form_new['output_options'] = array(
      '#type' => 'fieldset',
      '#title' => 'Options',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );    
    // add output options
    $output = $menu_content['output'];
    include_once MAXIMENU_PLUGINS . '/outputs/' . $output . '/' . $output . '.options.php';
    $output_function = '_maximenu_' . $output . '_options';
    $form_new['output_options'] += $output_function($menu_content, $settings); 
  }
  return $form_new;
}


/**
 * Admin Form: Delete
 *
 */
function _maximenu_delete($menu_id = NULL, $menu_content = array()) {
  $maximenu_array = array();
  $maximenu_array[$menu_id]['delete'] = array(
    '#type'  => 'hidden',
    '#default_value' => 1,
  );
  $maximenu_array[$menu_id]['output'] = array(
    '#type'  => 'hidden',
    '#default_value' => isset($menu_content['output']) ? $menu_content['output']: 'block',
  );
  return $maximenu_array;
}

    
/**
 * Admin Form: Submit
 *
 */
function maximenu_admin_submit($form, $form_state) {
  //dsm($form);
  global $_maximenu_variable;
  $maximenu = $_maximenu_variable;
  //dsm($form);
  //dsm($maximenu);
  $form_values = $form_state['values'];
  $maximenu_array = $form_state['values']['maximenu'];
  //dsm($maximenu_array);
  $maximenu_count = 0;
  $menu_id_last = 0;
  foreach ($maximenu_array as $menu_id => $maximenu_content) {
    if (!isset($maximenu_content['delete'])) $maximenu_content['delete'] = 0; 
    /**
     * Filters adding of menus from:
     * - 'New Menu' or empty menu
     * - Delete is checked
     * - is_numeric - prevents adding maximenu__active_tab
     */
    if (!empty($maximenu_content['title']) && ($maximenu_content['title'] != 'New Menu') && ($maximenu_content['delete'] != 1) && is_numeric($menu_id)) {
      $maximenu_count++;
      // cloned menus
      $menu_id_links = ($maximenu_content['menu_id_cloned'] != 0) ? $maximenu_content['menu_id_cloned']: $menu_id;
      
      $maximenu[$menu_id] = array(
        'code'   => $maximenu_content['code'],
        'title'  => $maximenu_content['title'],
        'style'  => isset($maximenu_content['style']) ? strtolower($maximenu_content['style']): '', 
        'output' => $maximenu_content['output'],
        'links'  => isset($maximenu[$menu_id_links]['links']) ? $maximenu[$menu_id_links]['links']: array(),       
      ); 
      foreach ($maximenu_content['style_options'] as $style => $style_val) {
        // prevents disabled options and empty option
        if (($style_val != 0) || ($style_val != '')) {
          $maximenu[$menu_id][$style] = $style_val;  
        }
      }
      foreach ($maximenu_content['output_options'] as $output => $output_val) {
        // prevents disabled options and empty option
        if (($output_val != 0) || ($output_val != '')) {
          $maximenu[$menu_id][$output] = $output_val;  
        }
      }
      if ($form_values['op'] == t('Cancel')) drupal_goto('admin/structure/maximenu');       
      drupal_set_message(t('%menu_title has been saved.', array('%menu_title' => check_plain($maximenu[$menu_id]['title']))));          
      $menu_id_last = $menu_id;
      _maximenu_update_blocks_db($menu_id, $maximenu_content);
    }
    elseif (isset($maximenu_content['delete']) && ($maximenu_content['delete'] == 1)) {
      if ($form_values['op'] == t('Cancel')) drupal_goto('admin/structure/maximenu'); 
      drupal_set_message(t('%menu_title has been deleted.', array('%menu_title' => check_plain($maximenu[$menu_id]['title']))));          
      unset($maximenu[$menu_id]); 
      _maximenu_update_blocks_db($menu_id, $maximenu_content);
    }
    elseif ($menu_id == 'maximenu__active_tab') {
      variable_set('maximenu_active_tab', $maximenu_content);
    }
  }
  //dsm($maximenu);
  // Save all settings in 1 variable
  variable_set('maximenu', $maximenu); 
  if ($maximenu_count == 1) {
    drupal_goto('admin/structure/maximenu/' . $menu_id_last . '/edit');
  }
  elseif ($_GET['q'] <> 'admin/structure/maximenu/maxedit') {
    drupal_goto('admin/structure/maximenu');
  }  
}





