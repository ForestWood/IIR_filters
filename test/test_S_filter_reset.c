/*
 * Copyright (C) 2018 Jose Marco de la Rosa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * Author: Jose Marco
 *
 * Created on January 20, 2018, 1:58 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <IIR_filters.h>
#include "load_test_reference_file.h"

#define STR_VALUE2(arg) #arg
#define STR_VALUE(name) STR_VALUE2(name)
#define IIR_SIGNAL_TYPE_STR STR_VALUE(IIR_SIGNAL_TYPE)


int main(int argc, char** argv) {
    if (argc != 2){
        printf( get_usage_str( argv[0] ) );
        return EXIT_FAILURE;
    }
    
    int error = 0;
    
    test_data_t *loaded_data = load_filter_test_data_fields( argv[1] );
    
    if ( loaded_data ){
        
        int n_coefs = loaded_data->n_coefs;
        int n_inputs = loaded_data->n_inputs;
        IIR_signal_t *a_coefs = loaded_data->a_coefs;
        IIR_signal_t *b_coefs = loaded_data->b_coefs;
        IIR_signal_t *inputs = loaded_data->inputs;

        // Create two filters
        IIR_S_t *filter1 = IIR_S_create( n_coefs, b_coefs, a_coefs );
        IIR_S_t *filter2 = IIR_S_create( n_coefs, b_coefs, a_coefs );
        
        // Feed one of them
        for ( int i=0; i < n_inputs; i++ ){
            IIR_S_add_input(filter1, inputs[i]);
        }
        
        // Reset
        IIR_S_reset( filter1 );
        
        // Now feed both and check that outputs match
        for ( int i=0; i < n_inputs; i++ ){            
            if (IIR_S_add_input(filter1, inputs[i]) != IIR_S_add_input(filter2, inputs[i])){
                error = 1;
                break;
            }
        }
        
        IIR_S_destroy(filter1);
        IIR_S_destroy(filter2);
        
        printf( "Using IIR_SIGNAL_TYPE %s\n", STR_VALUE(IIR_SIGNAL_TYPE) );
               
    }else{
        printf( "Error loading the test data file.\n" );
        return EXIT_FAILURE;
    }
        
    if (error){
        printf( "\nERROR: IIR_S: Filter reset does not work\n" );
        return EXIT_FAILURE;
    }

    printf( "\nSUCCESS: IIR_S: Filter reset does work\n" );
    return EXIT_SUCCESS;
}

