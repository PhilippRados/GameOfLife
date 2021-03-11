#include "./include/criterion/criterion.h"
#include "include/criterion/assert.h"
#include "include/criterion/internal/test.h"
#include "main.h" 

extern multi_arr Field;

Test(getLastMultiple,test_getLastMultiple_if_multiple_20){
    cr_assert(getLastMultiple(58) == 40);
    cr_assert(getLastMultiple(29) == 20);
    cr_assert(getLastMultiple(640) == 640);
}

Test(aliveNeighbors,test_count_alive_neighbors_if_cell_active){
    Field[20][10] = true;
    Field[19][10] = true;
    Field[21][11] = true;

    cr_assert(countAliveNeighbors(20, 10) == 2);
}

Test(aliveNeighbors,test_count_alive_neighbors_if_cell__not_active){
    Field[20][10] = true;
    Field[19][10] = true;
    Field[21][11] = true;

    cr_assert(countAliveNeighbors(20, 11) == 3);
}

Test(buttonPress,test_button_press_detected){
    cr_assert(buttonPress(2, HEIGHT) == true);
    cr_assert(buttonPress(5, HEIGHT) == false);
}

Test(copyArray,test_array_copied){
    Field[20][10] = true;
    Field[19][10] = true;
    Field[21][11] = true;

    multi_arr copied = {0,};

    copyArray(copied,Field);
    /*cr_assert_arrays_eq_cmp(copied,Field);*/
}

Test(nextEpoch,test_if_next_epoch_cells_correct_pos){
    int active_cells = 0;
    multi_arr nextState = {0,};

    Field[20][10] = true;
    Field[19][10] = true;
    Field[21][11] = true;

    nextEpoch();
    for (int i = 0;i < WIDTH;i++){
        for (int j = 0; j < HEIGHT;j++){
            active_cells += (Field[i][j] == true) ? 1 : 0;
        }
    }
    cr_assert(active_cells == 2);
    cr_assert(Field[20][10] == true && Field[20][11] == true);
}

