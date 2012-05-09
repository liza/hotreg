<?php
// $Id$

/**
 * @file
 * Maximenu Admin Links Configuration
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Admin Form: Links
 *
 */
function maximenu_links($form, &$form_state) {
  global $_maximenu_variable;  
  // counting active links
  $maximenu = $_maximenu_variable;
  // getting existing roles
  $result = db_query("SELECT rid, name, weight FROM {role} ORDER BY rid ASC");
  
  $roles = array();  
  while ($record = $result->fetchObject()) {
    $roles[$record->rid] = $record->name;
  }
  $args = arg();
  $menu_id = $args[3];
  $menu_content = $maximenu[$args[3]];
  drupal_set_title(t('Edit All') . ' ' . $menu_content['title'] . ' ' . t('Links'));  
    
  $active_links = isset($menu_content['links']) ? count($menu_content['links']): 0;

  $form = array();  
  $form['back'] = array(
    '#type'   => 'markup',
    '#markup' => '<h4>' . t('Active Links') . ' (' . $active_links . ') </h4>' .
       l(t('Maximenu'), 'admin/structure/maximenu', array('attributes' => array('title' => t('Go back to List of All Menus')))) . ' » ' . l($maximenu[$menu_id]['title'], 'admin/structure/maximenu/' . $menu_id . '/edit', array('attributes' => array('title' => t('Go back to ' . $maximenu[$menu_id]['title'])))) . ' » ' . 'All Links',
  );
  $form['menu_links'] = array(
    '#type'  => 'markup',
    '#markup' => '',
    '#tree'   => TRUE,
    '#theme' => '_maximenu_links',     
  ); 
  $form['menu_links'] += _maximenu_links($menu_content, $roles, $menu_id);
  $form['menu_id']['#value'] = $menu_id;
  $form['save'] = array( '#type' => 'submit', '#value' => t('Save'), );
  $form['cancel'] = array( '#type' => 'submit', '#value' => t('Cancel'), );   
  //dsm($form);   
  return $form;
}  


/**
 * Admin Form: Links submit
 *
 */
function maximenu_links_submit($form, $form_state) {
  global $_maximenu_variable;  

  $maximenu = $_maximenu_variable;
  $form_values = $form_state['values'];

  $links_array = $form_state['values']['menu_links'];
  $menu_id = $links_array['menu_id']['#value'];
  //dsm($links_array);                 
  foreach ($links_array as $link => $link_prop) {
    if (is_numeric($link)) {    
      if (!isset($link_prop['link_delete'])) $link_prop['link_delete'] = 0; 
      // links will not be included in the array if the title is not modified and delete field is checked
      if (!empty($link_prop['link_title']) && ($link_prop['link_title'] != 'New Link') && ($link_prop['link_delete'] == 0)) {
        $maximenu[$menu_id]['links'][$link] = array(
          'link_title' => $link_prop['link_title'],
          'link_title_option'  => $link_prop['title_options']['link_title_option'],
          'path_icon'          => $link_prop['title_options']['path_icon'],
          'icon_option'        => $link_prop['title_options']['icon_option'],  
          'php_option'         => $link_prop['title_options']['php_option'],
          'path'               => $link_prop['path'], 
          'path_query'         => $link_prop['path_query'], 
          'path_fragment'      => $link_prop['path_fragment'],             
          'weight'             => $link_prop['weight'],                                
          'id'                 => $link_prop['attributes']['id'],
          'class'              => $link_prop['attributes']['class'],
          'rel'                => $link_prop['attributes']['rel'],
          'target'             => $link_prop['attributes']['target'],                        
          'description'        => $link_prop['attributes']['description'],
          'description_option' => $link_prop['attributes']['description_option'],            
          'roles'              => $link_prop['permission']['roles'],            
          'content'    => isset($maximenu[$menu_id]['links'][$link]['content']) ? $maximenu[$menu_id]['links'][$link]['content']: '',
        );
        // stop and go back to menu editing
        if ($form_values['op'] == t('Cancel')) drupal_goto('admin/structure/maximenu/' . $menu_id . '/edit');       
        drupal_set_message(t('%link_title has been saved.', array('%link_title' => check_plain($maximenu[$menu_id]['links'][$link]['link_title']))));
      }
      elseif (($link_prop['link_delete'] == 1) && isset($menu_id)) { 
        if ($form_values['op'] == t('Cancel')) drupal_goto('admin/structure/maximenu/' . $menu_id . '/edit');       
        drupal_set_message(t('%link_title has been deleted.', array('%link_title' => check_plain($maximenu[$menu_id]['links'][$link]['link_title']))));               
        unset($maximenu[$menu_id]['links'][$link]);
      }
    }
  }

  // Save all settings in 1 variable
  variable_set('maximenu', $maximenu);     
}  

  
/**
 * Admin Form: Links
 *
 */
function _maximenu_links($menu_content = array(), $roles = array(), $menu_id = NULL) {
  
  if (!isset($menu_content['links'])) $menu_content['links'] = array();
  
  // This makes sure that even if existing links are deleted
  // it won't have an id below the existing ones
  // new link ids will depend on the highest existing link id;
  $count = !empty($menu_content['links']) ? max(array_keys($menu_content['links'])) + 1: 1;

  // sort by weight
  //uasort($menu_content['links'], 'om_sort_by_weight'); 
    
  $menu_content['links'] += _maximenu_new_link($count);
 

  $links = array();
  foreach ($menu_content['links'] as $link => $prop) {
    // count enabled blocks 
    $enabled_blocks = (isset($prop['content']) && !empty($prop['content'])) ? count($prop['content']): 0;  

    // field description attached text
    $attached       = ($enabled_blocks) ? ($enabled_blocks == 1) ? $enabled_blocks . ' block': $enabled_blocks . ' blocks': 'none';
    $edit_attached  = l(t('Edit'), 'admin/structure/maximenu/blocks/' . $menu_id . '/' . $link, 
      array(
        'attributes' => array(
          'title' => t('Enable / Disabled blocks attached to this link.'),
        ),
        //'query' => drupal_get_destination(),
        'html' => TRUE,
      )
    );
    $new_message = t('You can only enable/disable blocks on this link after you have modified the link title and saved.');    
    
    $link_title     = (isset($prop['link_title']) && !empty($prop['link_title'])) ? trim($prop['link_title']): 'New Link';
   // php on title
    if (isset($prop['php_option']) && ($prop['php_option'] == 1)) {
      ob_start();
      $link_title = eval($prop['link_title']);
      //$output = ob_get_contents();
      ob_end_clean(); 
    } 
    
    $links['menu_id'] = array(
      '#type'   => 'hidden',
      '#default_value'  => $menu_id,
    );    
    $links[$link] = array(
      '#type'   => 'fieldset',
      '#title'  =>  t($link_title),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
      '#description' => (!empty($link_title) && ($link_title != 'New Link')) ? $attached . ' | ' . $edit_attached: $new_message,         
    );
    
    // delete field is only for existing links
    if ($link_title != 'New Link') {
      $links[$link]['link_delete'] = array(
        '#type'   => 'checkbox',
        '#title'  => t('Delete this link.'),
        '#default_value'  => 0,      
      );    
    }
    $links[$link]['link_title'] = array(
      '#type'   => 'textarea',
      '#title'  => t('Link Title'),
      '#rows' => 1,
      '#required' => TRUE,        
      '#default_value'  => (isset($prop['link_title']) && !empty($prop['link_title'])) ? stripslashes(trim($prop['link_title'])): 'New Link',      
      '#description' => t('You can add inline styling or other attributes, ex. &lt;span class="special-link" style="font-weight: bold;"&gt;Home&lt;/span&gt;'),      
    ); 
    $links[$link]['title_options'] = array(
      '#type'   => 'fieldset',
      '#title'  => t('Title Options'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );      
    $links[$link]['title_options']['link_title_option'] = array(
      '#type'   => 'select',
      '#title'  => t('Options for Link Title'),
      '#default_value'  => isset($prop['link_title_option']) ? $prop['link_title_option']: 'title',
      '#options' => array('title' => 'Title only', 'title_icon' => 'Title with Icon', 'icon' => 'Icon only'),
    );        
    $links[$link]['title_options']['path_icon'] = array(
      '#type'   => 'textfield',
      '#title'  => t('Icon Path'),
      '#default_value'  => isset($prop['path_icon']) ? trim($prop['path_icon']): '',
      '#description' => t('
        Ex. sites/default/themes/mytheme/css/images/icon_1.png or<br />
        sites/default/files/icon_1.png <br />        
        (By default, this icon will go to the left side of the link title.). <br />
        This can also be done via css properties, ex .link-home { background: url(...)...'),      
    );  
    $links[$link]['title_options']['icon_option'] = array(
      '#type'   => 'checkbox',
      '#title'  => t('Icon Hover Image'),
      '#default_value'  => isset($prop['icon_option']) ? $prop['icon_option']: 0,
      '#description' => t('You can have a mouseover icon, just upload another image, Ex. icon.png, icon_hover.png, <br />
        just add "_hover" to the second image and this will automatically replace your active state icon on mouseover.'),      
    ); 
    $links[$link]['title_options']['php_option'] = array(
      '#type'   => 'checkbox',
      '#title'  => t('Title has PHP'),
      '#default_value'  => isset($prop['php_option']) ? $prop['php_option']: 0,
      '#description' => t('DO NOT include &lt;?php and ?&gt;, always add a return value, <br />
         ex. <strong> global $user; return \'&lt;strong&gt;\' . $user->name . \'&lt;/strong&gt;\';</strong>",<br />
         make sure your code is valid php, any error on this can potentially break your site.'),      
    );    
    $links[$link]['path'] = array(
      '#type'   => 'textfield',
      '#title'  => t('Path'),
      '#default_value'  => isset($prop['path']) ? trim($prop['path']): '',
      '#autocomplete_path' => module_exists('mpac') ? 'mpac/autocomplete': '',
      '#description' => t('
        <ul>
          <li><strong>Front Page</strong> - put &lt;front&gt; as path.</li>
          <li><strong>No Link</strong> - empty path is valid and will transform "a" tag to "span" tag.</li>
        </ul>  
          '),      
    );   
    $links[$link]['path_query'] = array(
      '#type'   => 'textfield',
      '#title'  => t('Path Query'),
      '#default_value'  => isset($prop['path_query']) ? trim($prop['path_query']): array(),
      '#description' => t('Ex. ?destination=node&me=you, but DO NOT include \'?\'.'),      
    ); 
    $links[$link]['path_fragment'] = array(
      '#type'   => 'textfield',
      '#title'  => t('Anchor'),
      '#default_value'  => isset($prop['path_fragment']) ? trim($prop['path_fragment']): '',
      '#description' => t('Ex. #section-1, but DO NOT include \'#\'. This can correspond to the id name of a tag, ex. &lt;div id="section-1",<br /> 
        so you can now target this section by doing "about#section-1", and the page will automatically scroll to that section.'), 
    );          
    $links[$link]['weight'] = array(
      '#type'   => 'weight',
      '#title'  => t('Weight'),
      '#delta' => 50,  
      '#attributes' => array('class' => array('weight')),             
      '#default_value'  => isset($prop['weight']) ? $prop['weight']: 0,
      '#description' => t('Negative values have the highest position in the order, so -1 comes first before 0 then 1, ...'),      
    ); 
    $links[$link]['attributes'] = array(
      '#type'   => 'fieldset',
      '#title'  => t('Link Attributes'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );                    
    $links[$link]['attributes']['id'] = array(
      '#type'   => 'textfield',
      '#title'  => t('ID'),
      '#default_value'  => isset($prop['id']) ? trim($prop['id']): '',
      '#description' => t('Make sure this ID is unique.'),      
    );
    $links[$link]['attributes']['class'] = array(
      '#type'   => 'textfield',
      '#title'  => t('Classes'),
      '#default_value'  => isset($prop['class']) ? trim($prop['class']): '',
      '#description' => t('It\'s a good practice to have classes in lowercase and dashes separating words.'),
    );
    $links[$link]['attributes']['rel'] = array(
      '#type'   => 'textfield',
      '#title'  => t('Relationship'),
      '#default_value'  => isset($prop['rel']) ? trim($prop['rel']): '',
      '#description' => t('
        <ul>
          <li><strong>nofollow</strong> - for search engines not to follow the link.</li>
          <li><strong>lightbox</strong> - if you installed Lighbox 2, see your other options on Lightbox 2 README.txt.</li>
          <li>Other modules may have uses for this, so I just kept it as textfield.</li>
        </ul> 
      '),            
    );  
    $links[$link]['attributes']['target'] = array(
      '#type'   => 'select',
      '#title'  => t('Target Window'),
      '#options' => array(
        '_self'   => 'Self', 
        '_blank'  => 'Blank',
        '_parent' => 'Parent',         
        '_top'    => 'Top',                                          
      ),
      '#default_value'  => isset($prop['target']) ? $prop['target']: '_self',
    );       
    $links[$link]['attributes']['description'] = array(
      '#type'   => 'textarea',
      '#title'  => t('Description'),
      '#default_value'  => isset($prop['description']) ? $prop['description']: '',
      '#description' => t('Link additional information'),            
    );
    $links[$link]['attributes']['description_option'] = array(
      '#type'   => 'select',
      '#title'  => t('Options for description'),
      '#options' => array('hover' => 'Hover', 'subtitle' => 'Subtitle'),
      '#default_value'  => isset($prop['description_option']) ? $prop['description_option']: 'hover',
      '#description' => t('
        <ul>
          <li><strong>Hover</strong> - appears on mouse over the link.</li>
          <li><strong>Subtitle</strong> - appears as text under the link.</a>.</li>
        </ul> 
      '),
    );  
    $links[$link]['permission'] = array(
      '#type'   => 'fieldset',
      '#title'  => t('Permission'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );         
    $links[$link]['permission']['roles'] = array(
      '#type'    => 'checkboxes',
      '#title'   => t('User Roles'),
      '#options' => $roles,
      '#default_value'  => isset($prop['roles']) ? $prop['roles']: array(),
      '#description' => t('If none is checked, this link will appear for all users.'),      
    );    
    //dsm($prop['content']);
    // these are the blocks
    if (isset($prop['content']) && is_array($prop['content'])) {
      foreach ($prop['content'] as $block => $block_prop) {
        foreach ($block_prop as $block_key => $block_val) {    
          $links[$link]['content'][$block][$block_key] = array(
            '#type'   => 'hidden',
            '#default_value'  => isset($block_val) ? trim($block_val): '',
          );
        }
      }
    }
  }
  return $links;
}


/**
 * Admin Form: New Links
 *
 */
function _maximenu_new_link($count = 0, $roles = array()) {

  $num = $count + 5; // additional 5 new blank links
  
  $out = array();
  for ($i = $count; $i < $num; $i++) {
    // new links with default values
    $out[$i] = array(
      'link_title'         => 'New Link',     
      'weight'             => 0,     
      'link_title_option'  => 'title',  
      'path_icon'          => '',                
      'icon_option'        => 0,
      'php_option'         => 0, 
      'path'               => '',      
      'path_query'         => '',      
      'path_fragment'      => '', 
      'weight'             => 0,  
      'id'                 => '',
      'class'              => '',
      'rel'                => '',
      'target'             => '_self',            
      'description'        => '',
      'description_option' => 'hover',      
      'roles'              => $roles,
    );
  }
  return $out;
}


/**
 * Implementation of theme_hook()
 * - View in table format
 *
 */
function theme__maximenu_links($form) {
  //dsm($form['form']);
  $form = $form['form'];
  $menu_id = $form['menu_id']['#value'];
  $output = '';
  //dsm($form);
  $rows = array();
  foreach ($form as $key => $prop) {
    if (is_numeric($key)) {
      $row = array();
      // Strips labels
      unset($prop['weight']['#title']);
      unset($prop['weight']['#description']);      
      unset($prop['link_delete']['#title']); 
      
      unset($form[$key]['#description']);      
      unset($form[$key]['weight']);      
      unset($form[$key]['link_delete']);                             
      $row[] = array('class' => 'col-link', 'data' => drupal_render($form[$key]));
      $row[] = array('class' => 'col-attached', 'data' => $prop['#description']);
      $row[] = array('class' => 'col-weight', 'data' => drupal_render($prop['weight']));
      $row[] = array('class' => 'col-delete', 'data' => drupal_render($prop['link_delete']));
            
      $rows[] = array(
        'data' => $row,
        'id' => 'row-' . $menu_id . '-' . $key,
        'class' => array('draggable row-' . $key, 'tabledrag-leaf'),
      ); 
    }
  }
  $header = array(t('Links Properties'), t('Attached Blocks'), t('Weight'), t('Delete')); //, 'Update'

  // Header
  $form['form']['#children'] = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' =>  array('id' => 'maximenu-links-' . $menu_id)));  

  //drupal_add_tabledrag($table_id, $action, $relationship, $group, $subgroup = NULL, $source = NULL, $hidden = TRUE, $limit = 0);
  drupal_add_tabledrag('maximenu-links-' . $menu_id, 'order', 'group', 'weight');
  
  $output .= drupal_render_children($form);
  
  return $output;
} 
