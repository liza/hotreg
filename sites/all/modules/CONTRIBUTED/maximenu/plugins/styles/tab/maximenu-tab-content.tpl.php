<?php
/**
 * @file maximenu-tab-content.tpl.php
 * Default theme implementation of om maximenu contents with tabbed blocks
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
 * @see template_preprocess_maximenu_tab()
 * @see template_preprocess_maximenu_tab_links()
 * @see template_preprocess_maximenu_tab_content()
 *
 */
?> 

<div class="maximenu-tab-content">
  <div class="maximenu-tab-content-inner">
    <?php foreach ($links['links'] as $key => $content): ?>
      <?php $permission = _maximenu_link_visible($content['roles']); ?>
      <?php if (!empty($permission) && !empty($content['content'])): ?>
        <div id="tab-content-<?php print $links['code'] . '-' . $key; ?>" class="tab-content tab-content-hide">
          <div class="tab-content-inner">
            <?php print _maximenu_content_render($content['content']); ?>
            <div class="clearfix"></div>
          </div><!-- /.tab-content-inner -->
        </div><!-- /.tab-content -->
      <?php endif; ?> 
    <?php endforeach; ?>
    <div class="clearfix"></div>
  </div><!-- /.maximenu-tab-content-inner -->
</div><!-- /.maximenu-tab-content -->
      

