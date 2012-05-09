<?php
/**
 * @file maximenu-accordion-content.tpl.php
 * Default theme implementation of om maximenu contents with accordion blocks
 *
 * Available variables:
 * - $content: link content blocks
 *
 * Helper variables:
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $user: (object) user properties
 *
 * @see template_preprocess_maximenu_accordion()
 * @see template_preprocess_maximenu_accordion_links()
 * @see template_preprocess_maximenu_accordion_content()
 *
 */
?>

<?php if (!empty($content)): ?>
  <div class="maximenu-accordion-content">
    <?php print _maximenu_content_render($content); ?>
    <div class="clearfix"></div>      
    <div class="maximenu-arrow"></div>
  </div><!-- /.maximenu-accordion-content -->
<?php endif; ?>


