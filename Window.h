#pragma once
#ifndef WINDOW_H

#include <gtkmm.h>

#include "Interval.h"
#include "funkcje.h"

using namespace interval_arithmetic;

enum DataType {
    FLOAT,
    INTERVAL,
    POINT_INTERVAL
};

class MyWindow : public Gtk::Window{
public:
    MyWindow();
    virtual ~MyWindow();

protected:
    void on_button_quit();
    void on_notebook_switch_page(Gtk::Widget* page, guint page_num);

    Gtk::Box m_VBox, m_RBBox;
    Gtk::Notebook m_Notebook;

    void on_dtype_rbutton_change();
    Gtk::RadioButton m_Float_RadioButton, m_Interval_RadioButton, m_PointInterval_RadioButton;
    Gtk::RadioButton m_Trimatrix_RadioButton, m_Symmatrix_RadioButton;

    Gtk::ButtonBox m_ButtonBox;
    Gtk::Button m_Button_Quit;

    Gtk::ButtonBox m_FileButtonBox;

    void on_entry_hmf_changed();
    Gtk::Label m_Label_How_Many_Functions;
    Gtk::Entry m_Entry_How_Many_Functions;

    //page 3 dane
    Gtk::Box m_DataBox;
    Gtk::Label m_Label_CurrentData;
    Gtk::Entry m_Entry_Data;
    Gtk::Button m_Button_NextData;
    void on_button_nextdata_clicked();

    //page 4
    Gtk::Box m_WynikBox;
    Gtk::Button m_Button_Oblicz;
    Gtk::Label m_Label_WynikNapis, m_Label_Wyniki;
    void on_button_oblicz_clicked();

private:
    unsigned int NOFunctions=3;
    int i=0,macierz=0,j=1;
    int dType = DataType::FLOAT;
    unsigned int dCounter = 0;

    std::vector<std::vector<long double>> float_matrix;
    std::vector<long double> float_a;
    std::vector<long double> float_b;
    std::vector<long double> float_c;
    std::vector<long double> float_d;
    std::vector<long double> float_x;

    //std::vector<std::vector<Interval<long double>>> interval_matrix;
    std::vector<Interval<long double>> interval_a;
    std::vector<Interval<long double>> interval_b;
    std::vector<Interval<long double>> interval_c;
    std::vector<Interval<long double>> interval_d;
    std::vector<Interval<long double>> interval_x;

    std::vector<Interval<long double>> interval_data;
    std::vector<long double> float_data;
};

#endif //WINDOW_H