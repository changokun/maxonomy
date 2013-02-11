<?php
require_once('maxonomy.inc');

function temp_autoload($class_name) {
	$file_to_look_for = 'C:/www/placewise/pw_php_sql/' . $class_name . '.inc';
	if(file_exists($file_to_look_for)) { include_once($file_to_look_for); return; }
}

spl_autoload_register('temp_autoload');

$widget = new widget(66); // see class def below

$list_of_continents = array('Africa', 'Asia', 'Europe', 'North America', 'South America');
$list_of_african_countries = array('Djibouti', 'Republic of the Congo', 'Mozambique', 'Libya', 'Egypt');


$continents_tag = new tag('Continents');
$countries_tag = new tag('Countries');

foreach($list_of_continents as $term) {
	$continent_tag = new tag($term);
	maxonomy::add_tags($continent_tag, $continents_tag);

	switch($term) {
		case 'Africa':
			foreach($list_of_african_countries as $country_name) {
				$country = new tag($country_name);
				maxonomy::add_tags($country, array($continent_tag, $countries_tag));
			}

		break;
	}
}


m::death(maxonomy::get_these_that_have_these_tags('tag', $continents_tag), '$continents');

//maxonomy::set_admin_mode(false);
//m::dump(maxonomy::get_direct_tags($widget), 'non admin');

// maxonomy::set_admin_mode(true);
// m::death(maxonomy::get_direct_tags($widget), 'admin');


class widget {
	const MAXONOMY_ID = 8;
	const MAXONOMY_PRIMARY_KEY_PROPERTY_NAME = 'widget_id';

	public function __construct($id) {
		$this->widget_id = $id;
	}

}