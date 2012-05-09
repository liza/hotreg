<?php
/**
 * @file maximenu-modal.tpl.php
 * Default theme implementation of om maximenu with modal blocks
 *
 * Available variables:
 * - $maximenu_name: Menu name given on configuration
 * - $disabled: Set links to be disabled when viewing the page of its path
 * - $links: All menu items which also contents each link property
 *
 * Helper variables:
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $user: (object) user properties
 * - $code: unique id given in the system
 * - $total: number of links
 *
 * @see template_preprocess_maximenu_modal()
 * @see template_preprocess_maximenu_modal_links()
 * @see template_preprocess_maximenu_modal_content()
 *
 */
?>  

<div id="menu-<?php print $maximenu_name; ?>-ul-wrapper" class="menu-ul-wrapper clearfix">
  <ul id="menu-<?php print $maximenu_name; ?>" class="menu">
    <?php foreach ($links['links'] as $key => $content): ?>
      <?php $count++; ?>
      <?php print theme('maximenu_modal_links', array('content' => $content, 'maximenu_name' => $maximenu_name, 'key' => $key, 'code' => $code, 'count' => $count, 'total' => $total)); ?>          
    <?php endforeach; ?>
  </ul><!-- /.menu -->    
</div><!-- /.menu-ul-wrapper -->   

    

