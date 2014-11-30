<?php

/**
 * Implements hook_node_view().
 */
function jmcouillard_attach_template_node_view($node, $view_mode, $langcode) {

    // Only override full view-mode
    if($view_mode == 'full') {

        // Render jmcouillard_attach_template template if select in node
        if(isset($node->attached_template) && !empty($node->attached_template)) {
            $template_name = $node->attached_template;
            include "templates/" . $template_name . ".inc";
            $template_settings = isset($node->attached_template_settings) ? $node->attached_template_settings : array();
            $node->content['jmcouillard_attach_template'] = array(
                "#theme" => "jmcouillard_attach_template_" . $template_name,
                "#data" => $template_name($template_settings)
            );
        }
    }
}

/**
 * Implements hook_field_extra_fields().
 */
function jmcouillard_attach_template_field_extra_fields() {

    foreach (entity_get_info() as $entity_type => $entity_info) {
        foreach (array_keys($entity_info['bundles']) as $bundle) {

                // This is a compatibilty fix for Display Suite and Metatag compatibilty
                // https://drupal.org/node/1284852
                if(module_exists('ds')) {
                    $extra[$entity_type][$bundle]['display']['metatags'] = array(
                        'label' => t('Meta tags'),
                        'description' => t('Rendered meta tags.'),
                        'weight' => 12,
                    );
                }

            if(_jmcouillard_attach_template_allowed_type($bundle)) {
                
                // Add render of template
                $extra['node'][$bundle]['display']['jmcouillard_attach_template'] = array(
                    'label' => t('Attached template'),
                    'description' => t('Render attached template.'),
                    'weight' => 10,
                );
            }
        }
    }

    return $extra;
}

/**
 * Implements hook_node_submit().
 */
function jmcouillard_attach_template_node_submit($node, $form, &$form_state) {

    global $language;

    if(isset($form_state['values'])) {
        $values = $form_state['values'];
        $lang = $language->language;

        // If a template was selected
        if(isset($values['attached_template'])) {

            // Save selected template
            $node->attached_template = $values['attached_template'];

            //
            // $node->cake_fullwidth = $values['cake_fullwidth'];

            // And save template settings
            // $options = _jmcouillard_attach_template_get_template_options($values['attached_template']);
            // foreach ($options as $key => $option) {
            // 	$node->attached_template_settings[$lang][$key] = $values[$key];
            // }

        }
    }
}

/**
 * Implements hook_node_insert().
 */
function jmcouillard_attach_template_node_insert($node) {

    // Save selected template
    if(isset($node->attached_template)) {
        if(!empty($node->attached_template)) {
            db_insert('jmcouillard_attach_template')->fields(array(
                'template' => $node->attached_template,
                // 'fullwidth' => $node->cake_fullwidth,
                'nid' => $node->nid,
            ))->execute();
        }
    }
}

/**
 * Implements hook_node_update().
 */
function jmcouillard_attach_template_node_update($node) {
    if(isset($node->attached_template)) {
        if(empty($node->attached_template)) {
            db_delete('jmcouillard_attach_template')->condition('nid', $node->nid)->execute();
        } else {
            $settings = isset($node->attached_template_settings) ? $node->attached_template_settings : array();
            db_merge('jmcouillard_attach_template')->fields(array(
                'template' => $node->attached_template,
                // 'fullwidth' => $node->cake_fullwidth,
                'nid' => $node->nid,
                'settings' => serialize($settings),
            ))->condition('nid', $node->nid)->execute();
        }
    }
}

/**
 * Implements hook_node_load().
 */
function jmcouillard_attach_template_node_load($nodes, $types) {

    // Load selected template into $node object
    $result = db_query('SELECT * FROM {jmcouillard_attach_template} WHERE nid IN(:nids)', array(':nids' => array_keys($nodes)))->fetchAllAssoc('nid');
    foreach ($nodes as &$node) {
        if(isset($result[$node->nid]->template)) {
            $node->attached_template = $result[$node->nid]->template;
            $node->attached_template_settings = unserialize($result[$node->nid]->settings);
        } else {
            $node->attached_template = FALSE;
            $node->attached_template_settings = array();
        }
    }
}

/**
 * Implements hook_form_node_form_alter().
 */
function jmcouillard_attach_template_form_node_form_alter(&$form, &$form_state) {

    if(!user_access('administer attached template')) {
        return;
    }

    global $language;
    $lang = $language->language;
    $type = $form['#node']->type;

    if(_jmcouillard_attach_template_allowed_type($type)) {

        // Define position in form
        $weight = isset($form['field_footer_extra_content']) ? $form['field_footer_extra_content']["#weight"] - 1 : 50;

        //  Create a fieldset to make it clean
        $form['template'] = array(
            '#type' => 'fieldset',
            '#title' => t('Template settings'),
            '#description' => t('You may link a template to your page here.'),
            '#weight' => $weight,
            '#collapsible' => TRUE,
            '#collapsed' => FALSE,
            '#multilingual' => TRUE,
        );

        // Add template selector
        $form['template'][$lang]['attached_template'] = array(
            '#type' => 'select',
            '#title' => t('Public page template'),
            '#empty_option' => "None",
            '#default_value' => isset($form['#node']->attached_template) ? $form['#node']->attached_template : 0,
            '#options' => _jmcouillard_attach_template_get_template_list(),
        );

    }

}

/**
 * Returns true if given type is allowed for attached templates
 */
function _jmcouillard_attach_template_allowed_type($type) {
    $allowed_types = variable_get('jmcouillard_attach_template_type', array('basic_page'));
    return isset($allowed_types[$type]) && $allowed_types[$type] != FALSE;
}
