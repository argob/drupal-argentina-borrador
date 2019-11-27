<?php

/**
 * Implementa hook_form_FORM_ID_alter().
 */
function drupar_borrador_form_node_form_alter(&$form, &$form_state) {
  if(isset($form['#node']->nid)) {
    $form['actions']['draft'] = array(
      '#type' => 'submit',
      '#class' => 'form-submit',
      '#value' => t('Guardar como Borrador'),
      '#weight' => '9',
      '#submit' => array('drupar_borrador_submit'),
    );
  }
}

/**
 * el Submit del borrador
 */
function drupar_borrador_submit($form, &$form_state) {
  #Da submit al formulario
  node_form_submit($form, $form_state);

  #Carga el nodo siendo editado
  $nid = arg(1);
  $node = node_load($nid);

  #Obtiene la lista de las revisiones del nodo
  $listaRevisiones = node_revision_list($node);

  #Obtiene (y elimina del array) la última revision
  $revisionPendienteDeAprobacion = array_shift($listaRevisiones);

  #Al eliminar la última revisión en el paso anterior, obtiene última revisión aprobada del nodo
  $revisionOriginal = array_shift($listaRevisiones);
  $revisionOriginal = node_load($nid, $revisionOriginal->vid);

  #Guarda revisión y define mensaje
  $revisionOriginal->revision = 1;
  $revisionOriginal->log = t('Copia según última revisión publicada. Borrador sujeto a aprobación.');

  #Guarda la copia de la última revisión publicada
  node_save($revisionOriginal);
}
