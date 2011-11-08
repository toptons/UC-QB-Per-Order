<?php

/**
 * @file
 * UC QB per order settings form
 *
 * Original code by toptons.com IT team
 */
function uc_qb_per_order_settings(&$form_state) {
  $uc_qb_per_order_settings = variable_get('uc_qb_per_order_settings', NULL);
  $uc_qb_per_order_qb_variables = variable_get('uc_qb_per_order_qb_variables', NULL);
  if ($uc_qb_per_order_qb_variables == NULL) {
    $form['uc_qb_per_order_pending'] = array(
        '#value' => t('QuickBooks variables are not ready, come back later.'),
        '#weight' => 0,
    );
  }
  else {
    //print_r(uc_taxes_rate_load());
    $qb_taxes = array();
    if ($uc_qb_per_order_qb_variables['qb_host_info']['country'] == 'US' && count($uc_qb_per_order_qb_variables['item_sales_tax'])) {
      $qb_taxes[''] = t('- select -');
      foreach ($uc_qb_per_order_qb_variables['item_sales_tax'] as $qtax) {
        $qb_taxes[$qtax['list_id']] = $qtax['name'] . "({$qtax['desc']})";
      }
    }
    elseif (count($uc_qb_per_order_qb_variables['sales_tax_code'])) {
      $qb_taxes[''] = t('- select -');
      foreach ($uc_qb_per_order_qb_variables['sales_tax_code'] as $qtax) {
        $qb_taxes[$qtax['list_id']] = $qtax['name'] . "({$qtax['desc']})";
      }
    }
    $form['uqpo_taxes_mapping'] = array(
        '#type' => 'fieldset',
        '#title' => t('Taxes mappings'),
        '#weight' => 5,
    );
    $w = 0;
    $uc_taxes = uc_taxes_rate_load();
    if (count($uc_taxes)) {
      foreach ($uc_taxes as $tax) {
        $form['uqpo_taxes_mapping'][$tax->id]['uc_tax'] = array(
            '#type' => 'hidden',
            '#value' => $tax->id,
            '#parents' => array(
                'uc_tax', $tax->id
            ),
        );
        $form['uqpo_taxes_mapping'][$tax->id]['qb_tax'] = array(
            '#type' => 'select',
            '#title' => t('@tax map to', array('@tax' => $tax->name)),
            '#default_value' => isset($uc_qb_per_order_settings['taxes'][$tax->id]) ? $uc_qb_per_order_settings['taxes'][$tax->id] : '',
            '#options' => $qb_taxes,
            '#weight' => $w,
            '#parents' => array(
                'qb_tax', $tax->id
            ),
            '#required' => TRUE,
        );
        $w++;
      }
    }
    $form['uc_item_acct_mapping'] = array(
        '#type' => 'fieldset',
        '#title' => t('UC items QuickBooks account default mapping'),
        '#weight' => 10,
    );
    $form['uc_item_acct_mapping']['cost_of_good_sold_acct'] = array(
        '#type' => 'select',
        '#title' => t('Cost of good sold account'),
        '#weight' => 0,
        '#default_value' => isset($uc_qb_per_order_settings['item_acct']['cogs']) ? $uc_qb_per_order_settings['item_acct']['cogs'] : '',
        '#options' => array('' => t('- select -')) + _get_qb_accounts('CostOfGoodsSold'),
        '#required' => TRUE,
    );
    $form['uc_item_acct_mapping']['income_acct'] = array(
        '#type' => 'select',
        '#title' => t('Income account'),
        '#weight' => 0,
        '#default_value' => isset($uc_qb_per_order_settings['item_acct']['income']) ? $uc_qb_per_order_settings['item_acct']['income'] : '',
        '#options' => array('' => t('- select -')) + _get_qb_accounts('Income'),
        '#required' => TRUE,
    );
    $form['uc_item_acct_mapping']['asset_acct'] = array(
        '#type' => 'select',
        '#title' => t('Asset account'),
        '#weight' => 0,
        '#default_value' => isset($uc_qb_per_order_settings['item_acct']['asset']) ? $uc_qb_per_order_settings['item_acct']['asset'] : '',
        '#options' => array('' => t('- select -')) + _get_qb_accounts('OtherCurrentAsset'),
        '#required' => TRUE,
    );

    $form['uc_receipt_invoice_acct_mapping'] = array(
        '#type' => 'fieldset',
        '#title' => t('QuickBooks receipt/invoice account mapping'),
        '#weight' => 12,
    );
    $form['uc_receipt_invoice_acct_mapping']['receipt_acct'] = array(
        '#type' => 'select',
        '#title' => t('Receipt deposit to account'),
        '#weight' => 0,
        '#default_value' => isset($uc_qb_per_order_settings['receipt_deposit_acct']) ? $uc_qb_per_order_settings['receipt_deposit_acct'] : '',
        '#options' => array('' => t('- select -')) + _get_qb_accounts('Bank'),
        '#required' => TRUE,
    );
    $form['uc_receipt_invoice_acct_mapping']['invoice_acct'] = array(
        '#type' => 'select',
        '#title' => t('Invoice to receivable account'),
        '#weight' => 0,
        '#default_value' => isset($uc_qb_per_order_settings['invoice_receivable_acct']) ? $uc_qb_per_order_settings['invoice_receivable_acct'] : '',
        '#options' => array('' => t('- select -')) + _get_qb_accounts('AccountsReceivable'),
        '#required' => TRUE,
    );

    $form['uc_special_item_mapping'] = array(
        '#type' => 'fieldset',
        '#title' => t('UC special item mapping'),
        '#weight' => 15,
    );
    $form['uc_special_item_mapping']['shipping'] = array(
        '#type' => 'select',
        '#title' => t('UC shipping map to'),
        '#default_value' => isset($uc_qb_per_order_settings['special_item']['shipping']) ? $uc_qb_per_order_settings['special_item']['shipping'] : '',
        '#weight' => 0,
        '#options' => array('' => t('- select -')) + _get_qb_item_other_charge(),
        '#required' => TRUE,
    );

    $form['uc_payment_method_mapping'] = array(
        '#type' => 'fieldset',
        '#title' => t('Payment method mapping'),
        '#weight' => 20,
    );
    $form['uc_payment_method_mapping']['payment_method'] = array(
        '#type' => 'select',
        '#title' => t('UC payment method map to'),
        '#default_value' => isset($uc_qb_per_order_settings['payment_method']) ? $uc_qb_per_order_settings['payment_method'] : '',
        '#weight' => 0,
        '#options' => array('' => t('- select -')) + _get_qb_payment_method(),
        '#required' => TRUE,
    );

    $form['uc_customer_message_mapping'] = array(
        '#type' => 'fieldset',
        '#title' => t('Customer message mapping'),
        '#weight' => 30,
    );
    $form['uc_customer_message_mapping']['customer_msg'] = array(
        '#type' => 'select',
        '#title' => t('UC customer message'),
        '#default_value' => isset($uc_qb_per_order_settings['customer_msg']) ? $uc_qb_per_order_settings['customer_msg'] : '',
        '#weight' => 0,
        '#options' => array('' => t('- select -')) + _get_qb_customer_message(),
        '#required' => TRUE,
    );

    $form['uc_class_mapping'] = array(
        '#type' => 'fieldset',
        '#title' => t('Class mapping'),
        '#weight' => 25,
    );
    $form['uc_class_mapping']['class'] = array(
        '#type' => 'select',
        '#title' => t('UC class'),
        '#default_value' => isset($uc_qb_per_order_settings['class']) ? $uc_qb_per_order_settings['class'] : '',
        '#weight' => 0,
        '#options' => array('' => t('- select -')) + _get_qb_class(),
        '#required' => FALSE,
    );

    $form['save_all_changes'] = array(
        '#type' => 'submit',
        '#value' => t('Save all changes'),
        '#weight' => 60
    );
    $form['reinitialize_qb_variables'] = array(
        '#type' => 'submit',
        '#value' => t('Re-initialize QuickBooks variables'),
        '#weight' => 65
    );
    $qb_ver = variable_get('qb_version', QB_VERSION_DEFAULT);
    $currency_opt = array();
    if (count($uc_qb_per_order_qb_variables['currency'])) {
      foreach ($uc_qb_per_order_qb_variables['currency'] as $c) {
        $currency_opt[$c['list_id']] = $c['name'] . "({$c['currency_code']})";
      }
    }
    $form['uc_qb_per_order_currency'] = array(
        '#type' => 'fieldset',
        '#title' => t('Currency'),
        '#weight' => 50,
    );
    $form['uc_qb_per_order_currency']['currency'] = array(
        '#type' => 'select',
        '#title' => t('Currency'),
        '#default_value' => isset($uc_qb_per_order_settings['currency']) ? $uc_qb_per_order_settings['currency'] : '',
        '#options' => $currency_opt,
        '#description' => t('Currency for this website.'),
        '#disabled' => (str_replace('.', '', $qb_ver) >= 80) ? FALSE : TRUE,
        '#required' => FALSE,
    );
  }

  return $form;
}

function _get_qb_class() {
  $uc_qb_per_order_qb_variables = variable_get('uc_qb_per_order_qb_variables', NULL);
  if ($uc_qb_per_order_qb_variables == NULL || count($uc_qb_per_order_qb_variables['class']) == 0) {
    return array();
  }
  $items = array();
  foreach ($uc_qb_per_order_qb_variables['class'] as $i) {
    $items[$i['list_id']] = $i['fullname'];
  }
  return $items;
}

function _get_qb_customer_message() {
  $uc_qb_per_order_qb_variables = variable_get('uc_qb_per_order_qb_variables', NULL);
  if ($uc_qb_per_order_qb_variables == NULL || count($uc_qb_per_order_qb_variables['customer_msg']) == 0) {
    return array();
  }
  $items = array();
  foreach ($uc_qb_per_order_qb_variables['customer_msg'] as $i) {
    $items[$i['list_id']] = $i['name'];
  }
  return $items;
}

function _get_qb_payment_method() {
  $uc_qb_per_order_qb_variables = variable_get('uc_qb_per_order_qb_variables', NULL);
  if ($uc_qb_per_order_qb_variables == NULL || count($uc_qb_per_order_qb_variables['payment_method']) == 0) {
    return array();
  }
  $items = array();
  foreach ($uc_qb_per_order_qb_variables['payment_method'] as $i) {
    $items[$i['list_id']] = $i['name'];
  }
  return $items;
}

function _get_qb_item_other_charge() {
  $uc_qb_per_order_qb_variables = variable_get('uc_qb_per_order_qb_variables', NULL);
  if ($uc_qb_per_order_qb_variables == NULL || count($uc_qb_per_order_qb_variables['item_other_charge']) == 0) {
    return array();
  }
  $items = array();
  foreach ($uc_qb_per_order_qb_variables['item_other_charge'] as $i) {
    $items[$i['list_id']] = $i['fullname'];
  }
  return $items;
}

function _get_qb_accounts($type = NULL) {
  $uc_qb_per_order_qb_variables = variable_get('uc_qb_per_order_qb_variables', NULL);
  if ($uc_qb_per_order_qb_variables == NULL || count($uc_qb_per_order_qb_variables['account']) == 0) {
    return array();
  }
  $acct = array();
  foreach ($uc_qb_per_order_qb_variables['account'] as $a) {
    $acct[$a['account_type']][$a['list_id']] = $a['fullname'];
  }
  if ($type != NULL) {
    return $acct[$type];
  }
  return $acct;
}

function uc_qb_per_order_settings_validate($form, &$form_state) {

}

function uc_qb_per_order_settings_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Re-initialize QuickBooks variables')) {
    drupal_goto('admin/settings/uc_qb_per_order/reinitialize_qb_variables/confirm', drupal_get_destination());
  }
  else {
    $uc_qb_per_order_settings = variable_get('uc_qb_per_order_settings', NULL);
    if (isset($form_state['values']['uc_tax'])) {
      foreach ($form_state['values']['uc_tax'] as $k => $v) {
        $uc_qb_per_order_settings['taxes'][$v] = $form_state['values']['qb_tax'][$v];
      }
    }
    $uc_qb_per_order_settings['item_acct']['cogs'] = $form_state['values']['cost_of_good_sold_acct'];
    $uc_qb_per_order_settings['item_acct']['income'] = $form_state['values']['income_acct'];
    $uc_qb_per_order_settings['item_acct']['asset'] = $form_state['values']['asset_acct'];
    $uc_qb_per_order_settings['receipt_deposit_acct'] = $form_state['values']['receipt_acct'];
    $uc_qb_per_order_settings['invoice_receivable_acct'] = $form_state['values']['invoice_acct'];
    $uc_qb_per_order_settings['special_item']['shipping'] = $form_state['values']['shipping'];
    $uc_qb_per_order_settings['currency'] = $form_state['values']['currency'];
    $uc_qb_per_order_settings['payment_method'] = $form_state['values']['payment_method'];
    $uc_qb_per_order_settings['customer_msg'] = $form_state['values']['customer_msg'];
    $uc_qb_per_order_settings['class'] = $form_state['values']['class'];
    variable_set('uc_qb_per_order_settings', $uc_qb_per_order_settings);
    drupal_set_message(t('Settings saved successfully.'));
  }
}

function reinitialize_qb_variables_confirm(&$form_state) {
  return confirm_form(NULL, t('Are you sure to re-initialize QuickBooks variables?'), $_GET['destination'], t('This action cannot be undone'), $yes = t('Yes'), $no = t('No'));
}

function reinitialize_qb_variables_confirm_submit($form, &$form_state) {
  variable_set('uc_qb_per_order_reset_qb_variables', 1);
  variable_del('uc_qb_per_order_qb_variables');
  drupal_set_message(t('QuickBooks variables will be rebuilt at next QBWC request.'));
}