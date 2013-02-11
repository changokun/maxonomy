<?php

require_once('tag.inc'); // these go together

// limitation: must use ints as ids for anything you want to tag.
// limitation: some keywords cannot be tags... mostly things that would cause empty() to return true, like false or 0
// in the class definition, add the consts.
// hmmm you can't always edit class files...... but then, why would you want to classify a class you didn't make?

// coupled to the pw_php_sql lib.

// todo - if a tag is created with matches in other languages, handle that.

class maxonomy {

	public static $preferred_language_locale = 'en'; // which really means en_US

	private static $admin_mode = false; // turn this on to work in all languages. otherwise works in preferred lang. changes outputs.
	private static $update_mode = false; // turn this on to allow instantiations to update records that have differences. empty fields are greedily filled in. see the tag construct for when it finds one record withm ultiple langs
	private static $add_mode = 'replace'; // replace (delete then insert) or append (blind - does not check for existing recs, will cause duplicates)

	const SCHEMA = 'test';
	const TABLE_NAME = 'maxonomy';

	public static function get_implicit_tags($object) {
		// i have this thiing, and it may or may not have any tags on it. please get me a list of all teh tags on it, and all the tags on those tags, and the tags on those tags, etc.
	}

	/**
	* get tags that are directly applied to the tagee.
	* in admin mode will return array of tag objects.
	* in non-admin mode will return array of tag_id => string (in preferred language)
	*
	* @param object $tagee
	*/
	public static function get_direct_tags($tagee) {
		// i have this thiing, and it may or may not have any tags on it. please get me a list of all teh tags directly on it.
		// need item_class_id and item_id
		$item_id = self::get_item_id_from_object($tagee);
		$item_class_id = self::get_class_id_from_object($tagee);

		// buld your query
		$query = new select_query('test', NULL, tag::SCHEMA . '.' . tag::TABLE_NAME . ' join ' . self::SCHEMA . '.' . self::TABLE_NAME . ' on ' . tag::TABLE_NAME . '.tag_id = ' . self::TABLE_NAME . '.tag_id', array('item_class_id = ' . $item_class_id, 'item_id = ' . $item_id));

		// return an array of tag_id => text in current language.
		// if in admin mode, return tag_id => array(en, es, ja);
		if(self::$admin_mode) {
			$query->clauses['select'] = '*';
			//return array(123 => array('oink', 'oink-o', 'oink-a'), 456 => array('moo', 'meerrrrruu'));
		} else {
			$query->clauses['select'] = array(self::TABLE_NAME . '.tag_id', self::$preferred_language_locale);
			//return array(123 => 'oink', 456 => 'moo');
		}

		// run the query
		if(false === $result = db::q($query)) throw new Exception('cannot load tags for this obj.');

		// groom it.
		if(self::$admin_mode) {
			$query->clauses['select'] = '*';
			// let's return an array of tag objects
			$ret = array();
			foreach($result as $row) $ret[$row['tag_id']] = new tag_internal_use_only($row);
			return $ret;
		} else {
			// transform it into array of id => string
			$ret = array();
			foreach($result as $row) $ret[$row['tag_id']] = $row[self::$preferred_language_locale];
			return $ret;
		}
	}

	public static function get_these_that_have_these_tags($class_name, $tags) {
		m::death($tags, $class_name);
	}

	private static function get_class_id_from_object($object) {
		if( ! defined(get_class($object) . '::MAXONOMY_ID')) throw new Exception(get_class($object) . 's are not prepared for maxonification. please add the required class constants, or provide the values as arguments.');
		return (int) $object::MAXONOMY_ID;
	}

	public static function add_tags($tagee, $tags, $add_mode = NULL) {
		// need an item_class_id, which reflects what it is - event, user, tag, etc.
		// for now, need an item_id as well, which is the primary key value for that thing. without it, the tags would apply to the class itself. which may be what you want.
		// we'll be adding a record for each tag id.
		// add these tags to this thing.
		// todo  add deferral?

		if(is_object($tagee)) {
			$item_id = self::get_item_id_from_object($tagee);
			$item_class_id = self::get_class_id_from_object($tagee);
		/*} elseif(is_array($tagee)) {
			$item_class_id = (int) array_shift($tagee);
			$item_id = (int) array_shift($tagee);
			// todo make sure we got good ints.*/
		} else {
			throw new Exception('bad tagee args.');
		}

		// if we don't have item class and id, we should have thrown exception by now.
		// so let's work on the list of tag_ids.
		if( ! is_array($tags)) $tags = array($tags);

		foreach($tags as $key => $tag) {
			if(is_scalar($tag)) {
				// create a tag obj
				$tags[$key] = new tag($tag);
			} elseif(is_object($tag) and get_class($tag) == 'tag') {
				// validate?
			} else {
				throw new Exception('bad tags.');
			}
		}

		// make the queries
		// are we replacing? or blindly adding? are we allowing duplicates (double tagging)?
		if(empty($add_mode)) $add_mode = self::$add_mode;
		if($add_mode = 'replace') {
			// that means clear out existing recs first
			$query = new delete_query('test', self::SCHEMA . '.' . self::TABLE_NAME, array('item_class_id = ' . $item_class_id, 'item_id = ' . $item_id));
			if(false === db::q($query)) throw new Exception('cannot remove tags from db.');
		}
		// either mode will now insert new records. blind appending will create duplicates. this may be a good thing.
		$data = array();

		foreach($tags as $tag) {
			$data_row = array('integer' => array());
			$data_row['integer'] = array('item_class_id' => $item_class_id, 'item_id' => $item_id, 'tag_id' => $tag->tag_id);
			$data[] = $data_row;
		}
		$query = new insert_query('test', self::SCHEMA . '.' . self::TABLE_NAME, $data);

		// m::dump($tags, 'class id:' . $item_class_id . ' | item id:' . $item_id);

		if(false === db::q($query)) throw new Exception('cannot insert max tags');

		return true;
	}


	private static function get_item_id_from_object($object) {
		if( ! defined(get_class($object) . '::MAXONOMY_PRIMARY_KEY_PROPERTY_NAME')) throw new Exception(get_class($object) . 's are not prepared for maxonification. please add the required class constants, or provide the values as arguments.');
		if(isset($object->{$object::MAXONOMY_PRIMARY_KEY_PROPERTY_NAME})) return (int) $object->{$object::MAXONOMY_PRIMARY_KEY_PROPERTY_NAME};
		return false;
	}

	public static function remove_tags($thing, $tags = 'clear all') {
		// remove these/all tags from this thing
	}

	public static function get_these_things_that_have_these_tags($type, $tags) {
		// get me a list of ids? or full objects? of this type (class name) with these tags.
	}

	/**
	* returns boolean indicating change accepted. false pretty much indicates that the language was not found. register new languages in the tag class.
	*/
	public static function set_preferred_language_locale($language_locale = NULL) {
		// defaults to en (_US)
		// maybe you want spanish tags, if they exist.
		// todo accept list/arry of langs
		if(in_array($language_locale, tag::$supported_languages)) {
			self::$preferred_language_locale = $language_locale;
		} elseif(in_array(substr($language_locale, 0, 2), tag::$supported_languages)) {
			self::$preferred_language_locale = substr($language_locale, 0, 2);
		} else {
			return false;
		}

		return true;

	}

	public static function set_admin_mode($enable = true) {
		// turning on admin mode will get you all languages, instead of just the preferred.
		self::$admin_mode = (bool) $enable;
	}

	public static function set_update_mode($enable = true) {
		// turning on update mode will tell tags to update on simple instantiation
		self::$update_mode = (bool) $enable;
	}

}

