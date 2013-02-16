<?php


/**
 *
 */
function jmcouillard_private_nodes_enterpassword( ) {
  
  $form['private_nodes_enterpassword'] = array(
    '#description' => "set"
  );
  
  $form['private_nodes_enterpassword']['password'] = array(
    '#type' => 'password',
    '#title' => t( 'Password'),
    '#size' => 20
  );
  
  $form['private_nodes_enterpassword']['submit'] = array(
    '#type' => 'submit',
    '#value' => t( 'Enter'),
  );
  return $form;
}


/**
 *
 */
function jmcouillard_private_nodes_enterpassword_validate($form, &$form_state) {

	//Define variables
	$fieldname = variable_get('private_nodes_field');
	$pass = $form_state['values']['password'];
	
	//Query
	$res = db_select("field_data_".$fieldname, 'p')
    	->fields('p', array('entity_id'))    	
   		->condition($fieldname.'_value', $pass ,'=') //WERE
    	->range(0,1) //LIMIT to 1 records
   	 	->execute()
   		->fetchAssoc();
	
	//Query as results
	if ($res >= 1) {
	
		//Avoid duplicate
		if(isset($_SESSION['private_nodes_passwords'])) {
			$_SESSION['private_nodes_passwords'] = array_unique($_SESSION['private_nodes_passwords']);
		}
		
		//Save new password to session
		$_SESSION['private_nodes_passwords'][] = $pass;
		
		//Redirect
		$path = drupal_get_path_alias("node/".$res['entity_id']);
		drupal_goto($path);
	
	//Query as no results	
	} else {
	
		//Invalid password
		form_set_error( 'password', t( 'Incorrect password.' ) );
	}
}

function jmcouillard_private_nodes_enterpassword_submit($form, &$form_state) {
 // $_SESSION['_protected_node']['passwords'][$_SESSION['_protected_node']['current']] = TRUE;
 // unset( $_SESSION['_protected_node']['current'] );
}

