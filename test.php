<?php
require_once('maxonomy.inc');


$widget = new widget();

maxonomy::add_tags($widget, array('oink', 'moo'));

m::dump(tag::get_tag_by_id(23));

m::death(new tag('shit'));
m::dump('ahou');
echo 'ahoy';


class widget {
	const MAXONOMY_ID = 8;
	const MAXONOMY_PRIMARY_KEY_PROPERTY_NAME = 'widget_id';

	public function __construct() {
		$this->widget_id = 77;
	}

}