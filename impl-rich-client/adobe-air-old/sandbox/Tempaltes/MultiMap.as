/*
 * MultiMap.as v1.1 - ActionScript 3.0 MultiMap Data Structure
 * written by Sascha Balkau - http://www.hiddenresource.net/
 * Based on Michael Baczynski's HashTable class.
 *
 * ``The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 */
package com.hexagonstar.data.structures.maps
{
    /**
     * A MultiMap works similar like a HashMap but let's the user map more
     * than one value to a key where the HashMap only allows one value
     * per key.
     * Mapped values can be fetched with the getValue method either returning
     * an Array that contains all values that are mapped to the key or only
     * one of the values defined by the valueIndex argument.
     * 
     * @example
     * <p><pre>
     *    // Creating a MultiMap with a size of 10000:
     *    _multiMap = new MultiMap(100000, MultiMap.hashInt);
     *    // Mapping values to the map:
     *    _multiMap.put(1000, "valueA_0", "valueB_0", "valueC_0", "valueD_0");
     *    _multiMap.put(1001, "valueA_1", "valueB_1", "valueC_1", "valueD_1");
     *    _multiMap.put(1002, "valueA_2", "valueB_2", "valueC_2", "valueD_2");
     *    // Fetching "valueC_1":
     *    var result:String = _multiMap.getValue(1001, 2);
     * </pre>
     */
    public class MultiMap
    {
        // Properties /////////////////////////////////////////////////////////////////
        
        /** @private */
        private var _map:Array;
        /** @private */
        private var _size:int;
        /** @private */
        private var _divisor:int;
        /** @private */
        private var _count:int;
        /** @private */
        private var _hash:Function;
        /** @private */
        private var _noDupeCheck:Boolean;
        
        // Constructor ////////////////////////////////////////////////////////////////
        
        /**
         * Constructs a new MultiMap instance.
         * 
         * @param size The size that the MultiMap should have.
         * @param hashFunction The hashing function used for the MultiMap.
         *        Use either MultiMap.hashString or MultiMap.hashInt for
         *        this depending on the type of key used.
         * @param allowDuplicates Normally the MultiMap does not allow to have duplicate
         *        keys in it but if it can be assured that all provided keys are
         *        unique this argument can be set to true which results in a faster
         *        execution when adding new entries to the map with put().
         */
        public function MultiMap(size:int, hashFunction:Function, noDupeCheck:Boolean = false)
        {
            _size = size;
            _hash = hashFunction;
            _noDupeCheck = noDupeCheck;
            clear();
        }
        
        // Hash Functions /////////////////////////////////////////////////////////////
        
        /**
         * Returns a hashcode for the specified key.
         * 
         * @return An Integer that can be used as a hash for the MultiMap.
         */
        public static function hashString(key:String):int
        {
            var result:int = 0;
            var len:int = key.length;
            for (var i:int = 0; i < len; i++)
            {
                result += (i + 1) * key.charCodeAt(i);
            }
            return result;
        }
        
        /**
         * A simple function for hashing integers.
         * 
         * @return An Integer that can be used as a hash for the MultiMap.
         */
        public static function hashInt(key:int):int
        {
            return hashString("" + key);
        }
        
        // Query Operations ///////////////////////////////////////////////////////////
        
        /**
         * Returns the amount of mappings that the map contains.
         *
         * @return The amount of mappings in the map.
         */
        public function size():int
        {
            return _count;
        }
        
        /**
         * Returns whether the map contains any mappings or not.
         * 
         * @return true If the map contains any mappings otherwise false.
         */
        public function isEmpty():Boolean
        {
            return (size() < 1);
        }
        
        /**
         * Returns the value that is mapped to the specified key and at the
         * specified valueIndex. If no valueIndex is specified always an
         * Array is returned that contains all values that are mapped to the
         * specified key, no matter if only one or several values were mapped.
         * If the valueIndex is larger than the resulting array's length
         * undefined is returned instead.
         * 
         * @param key The key to return the mapped value(s) for.
         * @param valueIndex The index in the resulting values array from
         *        which a single value should be returned. If this argument
         *        is omitted all mapped values are returned as an array.
         * @return Either an array containing all values mapped to the key
         *         or the specific value that exists at valueIndex or
         *         undefined if the valueIndex is out of range.
         */
        public function getValue(key:*, valueIndex:int = -1):*
        {
            if (valueIndex < 0) return getValues(key);
            
            var list:Array = _map[int(_hash(key) & _divisor)];
            var len:int = list.length;
            
            for (var i:int = 0; i < len; i++)
            {
                var entry:HashEntry = list[i];
                if (entry.key === key) return entry.data[valueIndex];
            }
            return null;
        }
        
        /**
         * Returns an array with all values that are mapped to the specified key.
         * 
         * @param key The key to return the mapped values for.
         * @return An array that contains all values that are mapped to the key.
         */
        public function getValues(key:*):Array
        {
            var list:Array = _map[int(_hash(key) & _divisor)];
            var len:int = list.length;
            
            for (var i:int = 0; i < len; i++)
            {
                var entry:HashEntry = list[i];
                if (entry.key === key)
                {
                    return entry.data;
                }
            }
            return null;
        }
        
        /**
         * Checks whether the MultiMap contains the specified key.
         * 
         * @param key The key to check for existance in the map.
         * @return true if the specified key exists in the MultiMap
         *         otherwise false.
         */
        public function containsKey(key:*):Boolean
        {
            var list:Array = _map[int(_hash(key) & _divisor)];
            var len:int = list.length;
            
            for (var i:int = 0; i < len; i++)
            {
                var entry:HashEntry = list[i];
                if (entry.key === key) return true;
            }
            return false;
        }
        
        /**
         * Checks in the MultiMap if it contains the specified value and
         * returns the mapped key of the first successful search result
         * it finds or returns -1 if the value wasn't found in the map.
         * 
         * @param value The value to be checked for availability in the map.
         * @return A number containing the key that the first successful
         *         search result is mapped to or -1 if the value was not
         *         found in the map.
         */
        public function containsValue(value:*):int
        {
            for (var i:int = 0; i < _size; i++)
            {
                var list:Array = _map[i];
                var len:int = list.length; 
                for (var j:int = 0; j < len; j++)
                {
                    var entry:HashEntry = list[j];
                    var l:int = entry.data.length;
                    for (var e:int = 0; e < l; e++)
                    {
                        if (entry.data[e] === value) return entry.key;
                    }
                }
            }
            return -1;
        }
        
        /**
         * Returns an associative array that contains the keys and
         * values of the MultiMap. The resulting array contains objects
         * with fields 'key' and 'data'. The data field contains an array
         * with all values that were mapped to the key.
         * 
         * @param sort If set to true the resulting array will be sorted
         *        on the 'key' field.
         * @return An associative array that contains the keys and
         *         values of the MultiMap.
         */
        public function toArray(sort:Boolean = false):Array
        {
            var result:Array = new Array();
            for (var i:int = 0; i < _size; i++)
            {
                var list:Array = _map[i];
                var len:int = list.length;
                if (len > 0)
                {
                    for (var j:int = 0; j < len; j++)
                    {
                        var entry:HashEntry = list[j];
                        result.push({key: entry.key, data: entry.data});
                    }
                }
            }
            return (sort) ? result.sortOn("key") : result;
        }
        
        /**
         * Returns a string representation of the MultiMap.
         * 
         * @return A string representation of the MultiMap.
         */
        public function toString():String
        {
            return "[MultiMap, size=" + size() + "]";
        }
        
        /**
         * Returns a string representing the MultiMap's structure.
         * Used for debugging purposes.
         * 
         * @return A string representing the MultiMap's structure.
         */
        public function dump():String
        {
            var result:String = "\nMultiMap:\n";
            for (var i:int = 0; i < _size; i++)
            {
                var list:Array = _map[i];
                var len:int = list.length;
                var idt:String = "";
                if (len > 0)
                {
                    result += i + ".\n";
                    for (var j:int = 0; j < len; j++)
                    {
                        if (j < 10) idt = "  ";
                        else if (j < 100) idt = " ";
                        var entry:HashEntry = list[j];
                        result += "\t" + idt + j + ". " + "key:" + entry.key
                            + "\t[" + entry.data + "]\n"
                    }
                }
            }
            return result;
        }
        
        // Modification Operations ////////////////////////////////////////////////////
        
        /**
         * Maps a key/values couple into the MultiMap. The key must be a string
         * or a positive integer for the MultiMap to work correctly. The value(s)
         * can be of any type. If the specified key already exists in the map, the
         * values on the key are replaced with the new values and true is returned.
         * If the key didn't exist in the map false is returned.
         * 
         * @param key A string or integer that acts as a key to map the value(s) with.
         * @param values A list of values that can be mapped to the given key.
         * @return true if the specified key already exists in the map and it's
         *         values were replaced with the new ones, otherwise false is returned.
         */
        public function put(key:*, ... values):Boolean
        {
            var pos:int = _hash(key) & _divisor;
            
            // Check for duplicate keys by default. If a dupe key is
            // found, replace it's value(s) with the new ones:
            if (!_noDupeCheck)
            {
                var list:Array = _map[pos];
                var len:int = list.length;
                for (var i:int = 0; i < len; i++)
                {
                    var entry:HashEntry = list[i];
                    if (entry.key === key)
                    {
                        entry.data = values;
                        return true;
                    }
                }
            }
            
            _map[pos].push(new HashEntry(key, values));
            _count++;
            return false;
        }
        
        /**
         * Removes the values from the MultiMap that are mapped with the specified
         * key or if a valueIndex larger than -1 was specified removes only the
         * value at the given index from the mapped key.
         * 
         * @param key The key from which to remove the mapped value(s) from.
         * @param valueIndex The index in the resulting values array from
         *        which a single value should be removed. If this argument
         *        is -1 or smaller all mapped values are removed.
         * @return Either an array containing all removed values mapped to the
         *         key or the specific value that existed at valueIndex or
         *         undefined if the valueIndex is out of range.
         */
        public function remove(key:*, valueIndex:int = -1):*
        {
            var list:Array = _map[int(_hash(key) & _divisor)];
            var len:int = list.length;
            var result:*;
            
            for (var i:int = 0; i < len; i++)
            {
                var entry:HashEntry = list[i];
                if (entry.key === key)
                {
                    if (valueIndex > -1)
                    {
                        result = entry.data[valueIndex];
                        entry.data.splice(valueIndex, 1);
                        return result;
                    }
                    else
                    {
                        result = entry.data;
                        list.splice(i, 1);
                        _count--;
                        return result;
                    }
                }
            }
            return null;
        }
        
        // Bulk Operations ////////////////////////////////////////////////////////////
        
        /**
         * Clears all mappings from the map. The map will be empty after this call.
         */
        public function clear():void
        {
            _map = new Array(_size);
            for (var i:int = 0; i < _size; i++)
            {
                _map[i] = [];
            }
            _divisor = _size - 1;
            _count = 0;
        }
    }
}

/**
 * Container class for storing key/data couples.
 */
internal class HashEntry
{
    public var key:int;
    public var data:Array;
    
    public function HashEntry(key:int, data:Array)
    {
        this.key = key;
        this.data = data;
    }
}
