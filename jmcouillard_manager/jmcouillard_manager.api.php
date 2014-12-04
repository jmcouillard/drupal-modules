<?php

/**
 * Allows the links at the bottom of the manager table.
 *
 * Implements hook_jmcouillard_mananger_actions_links_alter().
 *
 * @param $settings
 *   An associative array of Colorbox settings of links, included in the form
 */
function hook_jmcouillard_manager_actions_links_alter(&$actions, $type) {
	
	unset($actions["add"]);

	$form['actions']['custom_action'] = array(
		'#name' => 'custom_action',
		'#type' => 'submit',
		'#value' => t('My custom action'),
		'#submit' => array('my_function_callback'),
	);
}
