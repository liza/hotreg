<?php
/**
 * @file maximenu-modal-content.tpl.php
 * Default theme implementation of om maximenu contents with modal blocks
 *
 * Available variables:
 * - $links: link array
 *
 * Helper variables:
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $user: (object) user properties
 *
 * @see template_preprocess_maximenu_modal()
 * @see template_preprocess_maximenu_modal_links()
 * @see template_preprocess_maximenu_modal_content()
 *
 */
?> 

<?php foreach ($links['links'] as $key => $content): ?>
  <?php $permission = _maximenu_link_visible($content['roles']); ?>
    <?php if (!empty($permission) && !empty($content['content'])): ?>
      <div id="modal-content-<?php print $links['code'] . '-' . $key; ?>" class="maximenu-content maximenu-modal-content<?php print _maximenu_classes($links); ?>"<?php print _maximenu_inline_style($links); ?>>     
      <div class="maximenu-close">Close</div>  
      <div class="maximenu-top">
        <div class="maximenu-top-left"></div>
        <div class="maximenu-top-right"></div>
      </div><!-- /.maximenu-top --> 
      <div class="maximenu-middle">
        <div class="maximenu-middle-left">
          <div class="maximenu-middle-right">
            <?php print _maximenu_content_render($content['content']); ?>
            <div class="clearfix"></div>
          </div><!-- /.maximenu-middle-right --> 
        </div><!-- /.maximenu-middle-left --> 
      </div><!-- /.maximenu-middle --> 
      <div class="maximenu-bottom">
        <div class="maximenu-bottom-left"></div>
        <div class="maximenu-bottom-right"></div>
      </div><!-- /.maximenu-bottom -->  
    </div><!-- /.modal-content -->
  <?php endif; ?> 
<?php endforeach; ?>

 
