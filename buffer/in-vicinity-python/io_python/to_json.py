# coding: utf-8
import json

import io_wrapper as iow


def save_process_result(process_result, sets, raw):
    if raw:
        json_string = json.dumps(process_result)
    else:
        json_string = json.dumps(process_result, sort_keys=True, indent=2)
    to_file = [json_string]
    
    iow.list2file(sets, to_file)
    return json_string