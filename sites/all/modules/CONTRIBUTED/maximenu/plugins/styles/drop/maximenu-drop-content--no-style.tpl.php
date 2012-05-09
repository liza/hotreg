<?php
/**
 * @file maximenu-submenu-content.tpl.php
 * Default theme implementation of om maximenu contents with submenu blocks
 *
 * Available variables:
 * - $content: blocks
 *
 * Helper variables:
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $user: (object) user properties
 *
 * @see template_preprocess_maximenu_submenu()
 * @see template_preprocess_maximenu_submenu_links()
 * @see template_preprocess_maximenu_submenu_content()
 *
 */
?>  
<?php if (!empty($content)): ?>
  <div class="maximenu-content maximenu-content-nofade closed">
    <div class="maximenu-middle">
       <?php print _maximenu_content_render($content); ?>
       <div class="clearfix"></div>
    </div><!-- /.maximenu-middle --> 
    <div class="maximenu-open">
      <input type="checkbox" value="" />
      <?php print t('Stay'); ?>
    </div><!-- /.maximenu-open -->  
  </div><!-- /.maximenu-content -->  
<?php endif; ?> 

