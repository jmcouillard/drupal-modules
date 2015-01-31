<?php

/**
 *
 */
function jmcouillard_private_nodes_enterpassword() {

	$form['private_nodes_enterpassword'] = array('#description' => "set");

	$form['private_nodes_enterpassword']['password'] = array(
		'#type' => 'password',
		'#title' => t('Password'),
		'#size' => 20
	);

	$form['private_nodes_enterpassword']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Enter'),
	);
	return $form;
}

/**
 *
 */
function jmcouillard_private_nodes_enterpassword_validate($form, &$form_state) {

	// Define variables
	$fieldname = variable_get('jmcouillard_private_nodes_field');
	$pass = $form_state['values']['password'];

	// Query
	$query = db_select("field_data_" . $fieldname, 'p');
	$query->fields('p', array('entity_id'))->condition($fieldname . '_value', $pass, '=');
	$query->range(0, 1);
	$res = $query->execute()->fetchAssoc();

	// Query as no results
	if($res < 1) {
		form_set_error('password', t('Incorrect password.'));
	} else {
		drupal_static("redirect_entity_id", $res['entity_id']);
	}
}

function jmcouillard_private_nodes_enterpassword_submit($form, &$form_state) {

	$pass = $form_state['values']['password'];
	$entity_id = drupal_static("redirect_entity_id");

	//Avoid duplicate
	if(isset($_SESSION['jmcouillard_private_nodes_passwords'])) {
		$_SESSION['jmcouillard_private_nodes_passwords'] = array_unique($_SESSION['jmcouillard_private_nodes_passwords']);
	}

	//Save new password to session
	$_SESSION['jmcouillard_private_nodes_passwords'][] = $pass;

	//Redirect
	$path = drupal_get_path_alias("node/" . $entity_id);
	drupal_goto($path);
}
