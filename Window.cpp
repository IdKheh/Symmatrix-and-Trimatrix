#include "Window.h"
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <functional>

#include <dlfcn.h>

#define ld long double

using namespace std::placeholders;

void printInterval(Interval<ld> I)
{
    string left(""), right("");
    I.IEndsToStrings(left, right);
    std::cerr << left << " " << right << "\n";
}

MyWindow::MyWindow() : m_VBox(Gtk::ORIENTATION_VERTICAL),
                       m_RBBox(Gtk::ORIENTATION_VERTICAL),
                       m_Button_Quit("Wyjscie"),
                       m_Float_RadioButton("Zmiennoprzecinkowa"),
                       m_Interval_RadioButton("Przedzialowa"),
                       m_PointInterval_RadioButton("Punkt - przedzial"),
                       m_Symmatrix_RadioButton("Metoda macierzy symetrycznej"),
                       m_Trimatrix_RadioButton("Metoda Crouta"),
                       m_FileButtonBox(Gtk::ORIENTATION_VERTICAL),
                       m_Label_How_Many_Functions("ile ukladow rownan"),
                       m_DataBox(Gtk::ORIENTATION_HORIZONTAL),
                       m_Button_NextData("Dalej"),
                       m_Button_Oblicz("Oblicz"),
                       m_Label_WynikNapis("Wyniki: "),
                       m_Label_Wyniki(""),
                       m_WynikBox(Gtk::ORIENTATION_VERTICAL)
{

    set_title("Uklad rownan liniowych");
    set_border_width(10);
    set_default_size(400, 400);

    add(m_VBox);

    m_Notebook.set_border_width(10);
    m_VBox.pack_start(m_Notebook);
    m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

    m_Entry_How_Many_Functions.set_max_length(5);
    m_Entry_How_Many_Functions.set_text("3");
    m_Entry_How_Many_Functions.select_region(0, m_Entry_How_Many_Functions.get_text_length());
    m_Entry_How_Many_Functions.signal_changed().connect(sigc::mem_fun(*this, &MyWindow::on_entry_hmf_changed));

    auto group1 = m_Symmatrix_RadioButton.get_group();
    m_Trimatrix_RadioButton.set_group(group1);
    m_Symmatrix_RadioButton.set_active();

    m_Trimatrix_RadioButton.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_dtype_rbutton_change));
    m_Symmatrix_RadioButton.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_dtype_rbutton_change));

    m_FileButtonBox.pack_start(m_Symmatrix_RadioButton);
    m_FileButtonBox.pack_start(m_Trimatrix_RadioButton);
    m_FileButtonBox.pack_start(m_Label_How_Many_Functions);
    m_FileButtonBox.pack_start(m_Entry_How_Many_Functions);

    m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
    m_Button_Quit.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_button_quit));

    auto group = m_Float_RadioButton.get_group();
    m_Interval_RadioButton.set_group(group);
    m_PointInterval_RadioButton.set_group(group);

    m_Float_RadioButton.set_active();

    m_Float_RadioButton.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_dtype_rbutton_change));
    m_Interval_RadioButton.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_dtype_rbutton_change));
    m_PointInterval_RadioButton.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_dtype_rbutton_change));

    m_RBBox.pack_start(m_Float_RadioButton);
    m_RBBox.pack_start(m_Interval_RadioButton);
    m_RBBox.pack_start(m_PointInterval_RadioButton);

    // page 3
    std::stringstream ss;
    ss << "a[" << dCounter << "][" << dCounter << "]= ";
    auto label_cd = Glib::ustring(ss.str());
    m_Label_CurrentData = Gtk::Label(label_cd);
    m_DataBox.pack_start(m_Label_CurrentData);
    m_DataBox.pack_start(m_Entry_Data);
    m_Button_NextData.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_button_nextdata_clicked));
    m_DataBox.pack_start(m_Button_NextData);

    // page 4
    m_Button_Oblicz.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_button_oblicz_clicked));
    m_WynikBox.pack_start(m_Button_Oblicz);
    m_WynikBox.pack_start(m_Label_WynikNapis);
    m_WynikBox.pack_start(m_Label_Wyniki);

    m_Notebook.append_page(m_FileButtonBox, "Funkcje");
    m_Notebook.append_page(m_RBBox, "Typ danych");
    m_Notebook.append_page(m_DataBox, "Dane");
    m_Notebook.append_page(m_WynikBox, "Wyniki");

    m_Notebook.signal_switch_page().connect(sigc::mem_fun(*this, &MyWindow::on_notebook_switch_page));
    show_all_children();

    NOFunctions = strtoul(m_Entry_How_Many_Functions.get_text().c_str(), NULL, 10);

    float_matrix.clear();
    float_matrix.resize(NOFunctions);
    for (int i = 0; i < float_matrix.size(); i++)
        float_matrix[i].resize(NOFunctions);

    float_a.resize(NOFunctions);
    float_b.resize(NOFunctions);
    float_c.resize(NOFunctions);
    float_d.resize(NOFunctions);
    float_x.resize(NOFunctions);
    float_data.resize(NOFunctions * NOFunctions);
    /*
    interval_matrix.resize(NOFunctions);
    for (int i = 0; i < interval_matrix.size(); i++)
        interval_matrix[i].resize(NOFunctions);*/

    interval_a.resize(NOFunctions);
    interval_b.resize(NOFunctions);
    interval_c.resize(NOFunctions);
    interval_d.resize(NOFunctions);
    interval_x.resize(NOFunctions);
    interval_data.resize(NOFunctions * NOFunctions);
}

MyWindow::~MyWindow() {}

void MyWindow::on_button_quit() { hide(); }

void MyWindow::on_notebook_switch_page(Gtk::Widget *page, guint page_num) {}

void MyWindow::on_entry_hmf_changed() { NOFunctions = strtoul(m_Entry_How_Many_Functions.get_text().c_str(), NULL, 10); }

void MyWindow::on_dtype_rbutton_change()
{
    bool b_float = m_Float_RadioButton.get_active();
    bool b_interval = m_Interval_RadioButton.get_active();
    bool b_point_interval = m_PointInterval_RadioButton.get_active();
    bool b_Symmatrix = m_Symmatrix_RadioButton.get_active();
    bool b_Trimatrix = m_Trimatrix_RadioButton.get_active();

    dCounter = 0;

    float_matrix.clear();
    float_matrix.resize(NOFunctions);
    for (int i = 0; i < float_matrix.size(); i++)
        float_matrix[i].resize(NOFunctions);

    float_a.clear();
    float_a.resize(NOFunctions);
    float_b.clear();
    float_b.resize(NOFunctions);
    float_c.clear();
    float_c.resize(NOFunctions);
    float_d.clear();
    float_d.resize(NOFunctions);
    float_x.clear();
    float_x.resize(NOFunctions);
    float_data.clear();
    float_data.resize(NOFunctions * NOFunctions);

    interval_a.clear();
    interval_a.resize(NOFunctions);
    interval_b.clear();
    interval_b.resize(NOFunctions);
    interval_c.clear();
    interval_c.resize(NOFunctions);
    interval_d.clear();
    interval_d.resize(NOFunctions);
    interval_x.clear();
    interval_x.resize(NOFunctions);
    interval_data.clear();
    interval_data.resize(NOFunctions * NOFunctions);

    if ((b_float) && (b_Symmatrix))
    {
        dType = DataType::FLOAT;
        auto label_cd = Glib::ustring("a[0][0] = ");
        m_Label_CurrentData.set_text(label_cd);
        return;
    }
    else if (b_float && b_Trimatrix)
    {
        dType = DataType::FLOAT;
        auto label_cd = Glib::ustring("a[0] = ");
        m_Label_CurrentData.set_text(label_cd);
        return;
    }

    if ((b_interval) && (b_Symmatrix))
    {
        dType = DataType::INTERVAL;
        auto label_cd = Glib::ustring("a[0][0] = ");
        m_Label_CurrentData.set_text(label_cd);
        return;
    }
    else if ((b_interval) && (b_Trimatrix))
    {
        dType = DataType::INTERVAL;
        auto label_cd = Glib::ustring("a[0] = ");
        m_Label_CurrentData.set_text(label_cd);
        return;
    }

    if ((b_point_interval) && (b_Symmatrix))
    {
        dType = DataType::POINT_INTERVAL;
        auto label_cd = Glib::ustring("a[0][0] = ");
        m_Label_CurrentData.set_text(label_cd);
        return;
    }
    else if ((b_point_interval) && (b_Trimatrix))
    {
        dType = DataType::POINT_INTERVAL;
        auto label_cd = Glib::ustring("a[0] = ");
        m_Label_CurrentData.set_text(label_cd);
        return;
    }
}

void MyWindow::on_button_nextdata_clicked()
{
    setlocale(LC_ALL, "en-US");
    std::stringstream ss;
    std::size_t pos;
    bool b_Symmatrix = m_Symmatrix_RadioButton.get_active();
    bool b_Trimatrix = m_Trimatrix_RadioButton.get_active();

    // czy nie za duzo i bledy wpisania
    if ((dCounter > NOFunctions * NOFunctions + NOFunctions - 2) && (DataType::FLOAT) && (b_Symmatrix))
    {
        m_Label_CurrentData.set_text("Koniec");
        return;
    }
    else if ((dCounter > NOFunctions * NOFunctions + NOFunctions - 1) && ((DataType::POINT_INTERVAL) || (DataType::INTERVAL)))
    {
        m_Label_CurrentData.set_text("Koniec");
        return;
    }
    else if ((dCounter > NOFunctions * NOFunctions + NOFunctions - 4) && (DataType::FLOAT) && (b_Trimatrix))
    {
        m_Label_CurrentData.set_text("Koniec");
        return;
    }

    if ((dType == DataType::FLOAT) && (b_Symmatrix) && (dCounter < NOFunctions * NOFunctions))
    {
        auto s_val = m_Entry_Data.get_text();
        auto findAndReplaceAll = [](Glib::ustring &str, char *z, char *na)
        {
            size_t pos = str.find(z);

            while (pos != std::string::npos)
            {
                str.replace(pos, strlen(z), na);
                pos = str.find(z, pos + strlen(na));
            }
        };

        for (auto n : s_val)
        {
            switch (n)
            {
            case '.':
                findAndReplaceAll(s_val, ".", ",");
            case ',':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'e':
            case 'E':
            case 'd':
            case 'D':
                break;
            default:
                return;
            }
        }

        long double val = std::stold(s_val, &pos);

        float_data[dCounter++] = val;
        if (dCounter < NOFunctions * NOFunctions)
        {
            if (NOFunctions - 1 < j)
            {
                i++;
                j = 0;
            }
            ss << "a[" << i << "][" << j++ << "] = ";
        }
        else if (dCounter == NOFunctions * NOFunctions)
        {
            i = 0;
            j = 0;
            float_b[i++] = val;
            ss << "b[" << j++ << "] = ";
        }
        else
        {
            float_b[i++] = val;
        }

        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");
    }
    else if ((dType == DataType::FLOAT) && (b_Symmatrix) && (NOFunctions * NOFunctions <= dCounter < (NOFunctions * NOFunctions) + NOFunctions - 2))
    {

        auto s_val = m_Entry_Data.get_text();
        auto findAndReplaceAll = [](Glib::ustring &str, char *z, char *na)
        {
            size_t pos = str.find(z);

            while (pos != std::string::npos)
            {
                str.replace(pos, strlen(z), na);
                pos = str.find(z, pos + strlen(na));
            }
        };

        for (auto n : s_val)
        {
            switch (n)
            {
            case '.':
                findAndReplaceAll(s_val, ".", ",");
            case ',':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'e':
            case 'E':
            case 'd':
            case 'D':
                break;
            default:
                return;
            }
        }

        if (dCounter == NOFunctions * NOFunctions)
        {
            i = 0;
            j = 0;
        }

        long double val = std::stold(s_val, &pos);
        float_b[i++] = val;
        dCounter++;
        ss << "b[" << ++j << "]= ";
        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");
    }
    else if ((dType == DataType::POINT_INTERVAL) && (b_Symmatrix) && (dCounter < NOFunctions * NOFunctions))
    {

        auto s_val = m_Entry_Data.get_text();

        for (auto n : s_val)
        {
            switch (n)
            {
            case '.':
            case ',':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'e':
            case 'E':
            case 'd':
            case 'D':
                break;
            default:
                return;
            }
        }

        Interval<long double> tmp = IntRead<long double>(std::string(s_val));

        if (NOFunctions * NOFunctions - 1 >= dCounter)
        {
            interval_data[dCounter++] = tmp;
            if (NOFunctions - 1 < j)
            {
                i++;
                j = 0;
            }
            ss << "a[" << i << "][" << j++ << "] = ";
            std::cout << dCounter << "\n";
        }
        else
            dCounter++;

        // std::cout<<dCounter<<"\n";

        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");
    }
    else if ((dType == DataType::POINT_INTERVAL) && (b_Symmatrix) && (NOFunctions * NOFunctions < dCounter < (NOFunctions * NOFunctions) + NOFunctions + 5))
    {

        auto s_val = m_Entry_Data.get_text();

        for (auto n : s_val)
        {
            switch (n)
            {
            case '.':
            case ',':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'e':
            case 'E':
            case 'd':
            case 'D':
                break;
            default:
                return;
            }
        }
        Interval<long double> tmp = IntRead<long double>(std::string(s_val));
        if (dCounter == NOFunctions * NOFunctions)
        {
            i = 0;
            j = 0;
        }
        interval_b[i++] = tmp;
        ss << "b[" << j++ << "] = ";
        dCounter++;

        std::cerr << dCounter << "\n";
        std::cerr << i << " " << j << "\n";

        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");
    }

    else if ((dType == DataType::INTERVAL) && (b_Symmatrix) && (dCounter < NOFunctions * NOFunctions))
    {
        auto s_val = m_Entry_Data.get_text();
        std::cerr << s_val << '\n';
        for (auto n : s_val)
        {
            switch (n)
            {
            case '.':
            case ',':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'e':
            case 'E':
            case 'd':
            case 'D':
            case ' ':
                break;
            default:
                return;
            }
        }
        long double x = 0, y = 0;
        size_t sz;
        x = std::stold(s_val, &sz);
        y = std::stold(s_val.substr(sz));
        std::cerr << x << ' ' << y << '\n';
        interval_data[dCounter] = Interval<long double>(x, y);
        printInterval(interval_data[dCounter]);

        if (NOFunctions * NOFunctions - 1 >= dCounter)
        {
            dCounter++;
            // interval_data[dCounter++] = tmp;
            if (NOFunctions - 1 < j)
            {
                i++;
                j = 0;
            }
            ss << "a[" << i << "][" << j++ << "] = ";
            std::cout << dCounter << "\n";
        }
        else
            dCounter++;

        std::cerr << dCounter << "\n";

        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");
    }
    else if ((dType == DataType::INTERVAL) && (b_Symmatrix) && (NOFunctions * NOFunctions < dCounter < (NOFunctions * NOFunctions) + NOFunctions + 5))
    {

        auto s_val = m_Entry_Data.get_text();
        std::cerr << "b" << s_val << " " << dCounter << '\n';

        for (auto n : s_val)
        {
            switch (n)
            {
            case '.':
            case ',':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'e':
            case 'E':
            case 'd':
            case 'D':
            case ' ':
                break;
            default:
                return;
            }
        }
        long double x = 0, y = 0;
        size_t sz;
        x = std::stold(s_val, &sz);
        y = std::stold(s_val.substr(sz));
        std::cerr<<"i: "<<i<<"\n";
        if (dCounter == NOFunctions * NOFunctions)
        {
            i = 0;
            j = 0;
        }
        std::cerr<<"i': "<<i<<"\n";
        interval_b[i++] = Interval<long double>(x, y);
        printInterval(interval_b[i - 1]);
        
        // interval_b[i++] = tmp;

        ss << "b[" << j++ << "] = ";
        dCounter++;

        std::cerr << dCounter << "\n";
        std::cerr << i << " " << j << "\n";

        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");
    }

    else if ((dType == DataType::FLOAT) && (b_Trimatrix) && (dCounter < NOFunctions * NOFunctions + NOFunctions))
    {
        auto s_val = m_Entry_Data.get_text();
        auto findAndReplaceAll = [](Glib::ustring &str, char *z, char *na)
        {
            size_t pos = str.find(z);

            while (pos != std::string::npos)
            {
                str.replace(pos, strlen(z), na);
                pos = str.find(z, pos + strlen(na));
            }
        };

        for (auto n : s_val)
        {
            switch (n)
            {
            case '.':
                findAndReplaceAll(s_val, ".", ",");
            case ',':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'e':
            case 'E':
            case 'd':
            case 'D':
                break;
            default:
                return;
            }
        }

        long double val = std::stold(s_val, &pos);
        if (i == 0)
        {

            float_a[dCounter++] = val;
            ss << "a[" << dCounter << "] = ";
            std::cout << "a: " << dCounter - 1 << "\n";
            if (NOFunctions - 1 < dCounter)
            {
                i++;
                j = 0;
            }
        }
        else if (i == 1)
        {
            if (j == NOFunctions - 1)
            {
                float_b[j] = 0;
                i++;
                j = 0;
                ss << "c[" << j << "] = ";
            }
            else
            {
                float_b[j] = val;
                ss << "b[" << j++ << "] = ";
                std::cout << "b: " << j << "\n";
            }
            dCounter++;
        }
        else if (i == 2)
        {
            if (j == 0)
            {
                float_c[j] = 0;
                j++;
                ss << "c[" << j << "] = ";
            }
            else
            {
                float_c[j] = val;
                ss << "c[" << j++ << "] = ";
                std::cout << "c: " << j << "\n";
                if (NOFunctions - 1 < j)
                {
                    i++;
                    j = 0;
                }
            }
            dCounter++;
        }
        else if (i == 3)
        {
            float_d[j] = val;
            ss << "d[" << j++ << "] = ";
            std::cout << "d: " << j << "\n";
            if (NOFunctions - 1 < j)
            {
                i++;
                j = 0;
            }
            dCounter++;
        }
        else
        {
            m_Label_CurrentData.set_text("Koniec");
            return;
        }

        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");
    }

    else if ((dType == DataType::POINT_INTERVAL) && (b_Trimatrix) && (dCounter < NOFunctions * NOFunctions + NOFunctions))
    {

        auto s_val = m_Entry_Data.get_text();

        for (auto n : s_val)
        {
            switch (n)
            {
            case '.':
            case ',':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'e':
            case 'E':
            case 'd':
            case 'D':
                break;
            default:
                return;
            }
        }
        Interval<long double> tmp = IntRead<long double>(std::string(s_val));
        if (i == 0)
        {

            interval_a[dCounter++] = tmp;
            ss << "a[" << dCounter << "] = ";
            std::cout << "a: " << dCounter - 1 << "\n";
            if (NOFunctions - 1 < dCounter)
            {
                i++;
                j = 0;
            }
        }
        else if (i == 1)
        {
            if (j == NOFunctions - 1)
            {
                interval_b[j] = Interval<long double>(0, 0);
                i++;
                j = 0;
                ss << "c[" << j << "] = ";
            }
            else
            {
                interval_b[j] = tmp;
                ss << "b[" << j++ << "] = ";
                std::cout << "b: " << j << "\n";
            }
            dCounter++;
        }
        else if (i == 2)
        {
            if (j == 0)
            {
                interval_c[j] = Interval<long double>(0, 0);
                j++;
                ss << "c[" << j << "] = ";
            }
            else
            {
                interval_c[j] = tmp;
                ss << "c[" << j++ << "] = ";
                std::cout << "c: " << j << "\n";
                if (NOFunctions - 1 < j)
                {
                    i++;
                    j = 0;
                }
            }
            dCounter++;
        }
        else if (i == 3)
        {
            interval_d[j] = tmp;
            ss << "d[" << j++ << "] = ";
            std::cout << "d: " << j << "\n";
            if (NOFunctions - 1 < j)
            {
                i++;
                j = 0;
            }
            dCounter++;
        }
        else
        {
            m_Label_CurrentData.set_text("Koniec");
            return;
        }

        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");
    }
    else if ((dType == DataType::INTERVAL) && (b_Trimatrix) && (dCounter < NOFunctions * NOFunctions + NOFunctions))
    {

        auto s_val = m_Entry_Data.get_text();

        for (auto n : s_val)
        {
            switch (n)
            {
            case '.':
            case ',':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'e':
            case 'E':
            case 'd':
            case 'D':
            case ' ':
                break;
            default:
                return;
            }
        }
        ld x,y;
        size_t sz;
        x = std::stold(s_val, &sz);
        y = std::stold(s_val.substr(sz));
        Interval<long double> tmp = Interval<ld>(x,y);
        if (i == 0)
        {

            interval_a[dCounter++] = tmp;
            ss << "a[" << dCounter << "] = ";
            std::cout << "a: " << dCounter - 1 << "\n";
            if (NOFunctions - 1 < dCounter)
            {
                i++;
                j = 0;
            }
        }
        else if (i == 1)
        {
            if (j == NOFunctions - 1)
            {
                interval_b[j] = Interval<long double>(0, 0);
                i++;
                j = 0;
                ss << "c[" << j << "] = ";
            }
            else
            {
                interval_b[j] = tmp;
                ss << "b[" << j++ << "] = ";
                std::cout << "b: " << j << "\n";
            }
            dCounter++;
        }
        else if (i == 2)
        {
            if (j == 0)
            {
                interval_c[j] = Interval<long double>(0, 0);
                j++;
                ss << "c[" << j << "] = ";
            }
            else
            {
                interval_c[j] = tmp;
                ss << "c[" << j++ << "] = ";
                std::cout << "c: " << j << "\n";
                if (NOFunctions - 1 < j)
                {
                    i++;
                    j = 0;
                }
            }
            dCounter++;
        }
        else if (i == 3)
        {
            interval_d[j] = tmp;
            ss << "d[" << j++ << "] = ";
            std::cout << "d: " << j << "\n";
            if (NOFunctions - 1 < j)
            {
                i++;
                j = 0;
            }
            dCounter++;
        }
        else
        {
            m_Label_CurrentData.set_text("Koniec");
            return;
        }
        std::cout<<dCounter<<"\n";

        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");
    }
}

void MyWindow::on_button_oblicz_clicked()
{

    stringstream ss;
    std::cerr << "jestem1\n";
    Gtk::MessageDialog flagdialog(*this, "Error");
    flagdialog.set_secondary_text("Cos poszlo nie tak.\n ");
    switch (dType)
    {
    case DataType::FLOAT:
    {
        bool b_Symmatrix = m_Symmatrix_RadioButton.get_active();
        bool b_Trimatrix = m_Trimatrix_RadioButton.get_active();
        if (b_Symmatrix)
        {
            int k = 0;
            for (int l = 0; l < NOFunctions; l++)
                for (int m = 0; m < NOFunctions; m++)
                {
                    float_matrix[l][m] = float_data[k];
                    k++;
                }
        }
        if ((b_Trimatrix) && zwykla::trimatrix(NOFunctions, float_a, float_b, float_c, float_d) && (NOFunctions >= 1))
        {
            for (int i = 0; i < NOFunctions; i++)
                ss << "d[" << i << "] = " << float_d[i] << "\n";

            ss.clear();
        }

        else if ((b_Symmatrix) && (zwykla::wyznacznik(float_matrix, NOFunctions)) && (zwykla::symetryczna(float_matrix, NOFunctions)) && (NOFunctions >= 1))
        {
            zwykla::symmatrix(NOFunctions, float_matrix, float_b, float_x);
            for (int i = 0; i < NOFunctions; i++)
                ss << "x[" << i << "] = " << float_x[i] << "\n";
        }

        if ((b_Symmatrix) && (zwykla::wyznacznik(float_matrix, NOFunctions)) && (zwykla::symetryczna(float_matrix, NOFunctions)) && (NOFunctions >= 1))
            m_Label_Wyniki.set_text(ss.str());

        else if ((b_Trimatrix) && zwykla::trimatrix(NOFunctions, float_a, float_b, float_c, float_d) && (NOFunctions >= 1))
            m_Label_Wyniki.set_text(ss.str());
        else
            flagdialog.run();

        ss.clear();
    }
    break;
    case DataType::INTERVAL:
    case DataType::POINT_INTERVAL:
    {
        bool b_Symmatrix = m_Symmatrix_RadioButton.get_active();
        bool b_Trimatrix = m_Trimatrix_RadioButton.get_active();
        vector<vector<Interval<long double>>> interval_cos(NOFunctions, vector<Interval<long double>>(NOFunctions, Interval<long double>(0, 0)));

        if ((b_Symmatrix))
        {
            int k = 0;

            for (int l = 0; l < NOFunctions; l++)
            {
                for (int m = 0; m < NOFunctions; m++)
                {
                    interval_cos[l][m].a = interval_data[k].a;
                    interval_cos[l][m].b = interval_data[k].b;
                    printInterval(interval_cos[l][m]);
                    k++;
                }
            }

            for (int m = 0; m < NOFunctions; m++)
            {
                printInterval(interval_b[m]);
            }
        }
        if ((b_Trimatrix) && (przedzial::trimatrix(NOFunctions, interval_a, interval_b, interval_c, interval_d) && (NOFunctions >= 1)))
        {

            for (int i = 0; i < NOFunctions; i++)
            {
                std::string left, right;
                interval_d[i].IEndsToStrings(left, right);
                ss << "d[" << i << "] = " << left << " " << right << " szerokosc: " << IntWidth(interval_d[i]) << "\n";
            }

            ss.clear();
        }

        else if ((b_Symmatrix) && (przedzial::wyznacznik(interval_cos, NOFunctions)) && (przedzial::symetryczna(interval_cos, NOFunctions)) && (NOFunctions >= 1))
        {

            przedzial::symmatrix(NOFunctions, interval_cos, interval_b, interval_x);

            for (int i = 0; i < NOFunctions; i++)
            {
                std::string left, right;
                interval_x[i].IEndsToStrings(left, right);
                ss << "x[" << i << "] = " << left << " " << right << " szerokosc: " << IntWidth(interval_x[i]) << "\n";
            }
        }

        if ((b_Symmatrix) && (przedzial::wyznacznik(interval_cos, NOFunctions)) && (przedzial::symetryczna(interval_cos, NOFunctions)) && (NOFunctions >= 1))
            m_Label_Wyniki.set_text(ss.str());
        else if ((b_Trimatrix) && (przedzial::trimatrix(NOFunctions, interval_a, interval_b, interval_c, interval_d) && (NOFunctions >= 1)))
            m_Label_Wyniki.set_text(ss.str());
        else
            flagdialog.run();

        ss.clear();
    }

    break;
    default:
        std::cerr << "?\n";
        break;
    }
}