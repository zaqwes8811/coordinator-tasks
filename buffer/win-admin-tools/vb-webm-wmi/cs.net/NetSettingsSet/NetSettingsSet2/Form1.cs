using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Management;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using System.Xml;
using System.Xml.XPath;
using System.IO;
/**
 * Логика работы: 
 * 1. Во время инициализации загружаются настройки из xml-файла и читаются текущие 
 *   настройки сети.
 */

namespace NetSettingsSet2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            button2.Enabled = false;
            button4.Enabled = false;
            getNetWorkState(false, false);
            if (!loadSaveNetSet(true)) {
                loadSaveNetSet(true);  // возникло исключение и нужно повторить
            } 

        }
        /// <summary>
        /// Очень большой метод
        /// </summary>
        /// <param name="sets"></param>
        /// <param name="LoadForForm"></param>
        private void getNetWorkState(bool sets, bool LoadForForm) {
          try {
              ManagementClass objMC_ = new ManagementClass(
                  "Win32_NetworkAdapterConfiguration");
              ManagementObjectCollection objMOC_ = objMC_.GetInstances();
              // начало перебора
              int NumNic = 0; int NumDHCP = 0;
              foreach (ManagementObject objMO in objMOC_) {
                if (!(bool)objMO["ipEnabled"])
                    continue;
                // адаптер
                if (!sets)
                {
                    if (!LoadForForm) listBox1.Items.Add("Адаптер: ");
                    if (!LoadForForm) listBox1.Items.Add(objMO["Caption"]);
                    if (!LoadForForm) listBox1.Items.Add(objMO["MACAddress"]);
                }
                // сетевые настройки
                string[] ipaddresses = (string[])objMO["IPAddress"];
                string[] subnets = (string[])objMO["IPSubnet"];
                string[] gateways = (string[])objMO["DefaultIPGateway"];
                // IP-адрес
                if (ipaddresses[0] != "0.0.0.0")
                {
                    /// вводим настройки если задано
                    if (sets)
                    {
                        /// /// настройка ip-адреса и маски подсети
                        ManagementBaseObject setIP;
                        ManagementBaseObject newIP = objMO.GetMethodParameters("EnableStatic");
                        newIP["IPAddress"] = new string[] { textBoxIP.Text };
                        newIP["SubnetMask"] = new string[] { textBoxSM.Text };

                        setIP = objMO.InvokeMethod("EnableStatic", newIP, null);
                        if (setIP["returnValue"].ToString() == "0")
                        {
                            listBox1.Items.Add("IP-адрес и маска подсети установлены");
                        }
                        else
                        {
                            listBox1.Items.Add("Ошибка установки IP-адреса и маски подсети.");
                            listBox1.Items.Add("Воспользуйтесь стандартынм методом настройки.");
                        }
                        /// настройка шлюза
                        ManagementBaseObject setGateway;
                        ManagementBaseObject newGateway =
                            objMO.GetMethodParameters("SetGateways");

                        newGateway["DefaultIPGateway"] = new string[] { textBoxGW.Text };
                        newGateway["GatewayCostMetric"] = new int[] { 1 };

                        setGateway = objMO.InvokeMethod("SetGateways", newGateway, null);
                        // проверка возвращаемого занчения
                        if (setGateway["returnValue"].ToString() == "0")
                        {
                            listBox1.Items.Add("IP-адрес шлюза установлены.");
                        }
                        else
                        {
                            listBox1.Items.Add("Ошибка установки IP-адреса шлюза.");
                            listBox1.Items.Add("Воспользуйтесь стандартынм методом настройки.");
                        }
                    }
                    else
                    {
                        /// читаме настройки
                        bool dhcp_ena = (bool)objMO["DHCPEnabled"];
                        if (dhcp_ena)
                        {
                            listBox1.Items.Add("Включена служба DHCP.");
                            NumDHCP++;
                        }
                        else
                        {
                            // ip
                            if (ipaddresses != null)
                            {
                                if (LoadForForm) textBoxIP.Text = ipaddresses[0];
                                else listBox1.Items.Add("IP-адрес:" + ipaddresses[0]);
                            }
                            // маска
                            if (subnets != null)
                            {
                                if (LoadForForm) textBoxSM.Text = subnets[0];
                                else listBox1.Items.Add("Маска подсети:" + subnets[0]);

                            }
                            // шлюз
                            if (gateways != null)
                            {
                                if (LoadForForm) textBoxGW.Text = gateways[0];
                                else listBox1.Items.Add("IP-адрес шлюза по умолчанию:" + gateways[0]);

                            }
                            else
                            {
                                listBox1.Items.Add("IP-адрес шлюза по умолчанию не назначен.");
                            }
                        }
                        NumNic++;  //
                    }  // if(sets)..
                }  // if (ipad..
              }  // foreach (Mana..
              if (!sets)
              {
                  if (!LoadForForm) listBox1.Items.Add("Всего активных подключений: " + NumNic.ToString());
                  switch (NumNic)
                  {  // итого по сетям
                      case 1:
                          {
                              if (NumDHCP == 0)
                              {
                                  if (!LoadForForm)
                                      listBox1.Items.Add("Активное подключение одно и оно статическое.");
                                  if (!LoadForForm) 
                                      listBox1.Items.Add("Настройки можно свободно менять.");
                                  else
                                      listBox1.Items.Add("Настройки считаны.");
                                  button2.Enabled = true;
                                  button4.Enabled = true;
                              }
                              else
                              {
                                  listBox1.Items.Add("DHCP для работы должен быть отключен.");
                                  listBox1.Items.Add("Из данной программы сделать это так, чтобы");
                                  listBox1.Items.Add("соединение работало стабильно, нельзя.");
                                  listBox1.Items.Add("Действия: оключаем DHCP, вводим в ручную настройки,");
                                  listBox1.Items.Add("загружаем их в программу.");
                                  listBox1.Items.Add("Подробно:");
                                  listBox1.Items.Add("- Открываем <Панель управления>-><Сетевые подключения>");
                                  listBox1.Items.Add("- Выбираем <подключение>-><Свойства>->");
                                  listBox1.Items.Add("  <Протокол Интернета TCP/IP>-><Свойства>");
                                  listBox1.Items.Add("  Выбираем <Использовать следующий IP-адрес>");
                                  listBox1.Items.Add("- Вводим туда настройки, полученные от администратора.");
                                  listBox1.Items.Add("В данной программе нажимаем <Получить текущее состояние>");
                                  listBox1.Items.Add("Установленные настройки отобразятся на форме, ");
                                  listBox1.Items.Add("и сохранятся в файле <cfg.xml>. Закрываем окно и входим в программу");
                                  listBox1.Items.Add("управления передатчиком. Жмем <Обновить>. Теперь данные на форме");
                                  listBox1.Items.Add("основного приложения соответсвуют настройкам сетевого адаптера.");
                              }
                          } break;
                      case 0:
                          {
                              listBox1.Items.Add("Активных подключений не обранужено");
                          } break;
                      default:
                          {
                              listBox1.Items.Add("Найдено несколько активных соединения по TCP/IP.");
                              listBox1.Items.Add("Возможно данный компьютер настроен как маршрутизатор.");
                          } break;
                  }  // switch (Num..
              }  // if(sets..)
           }  // try {...
          catch(Exception err){
              listBox1.Items.Add("Исключение: " + err.ToString());
           }
        
        }
        
        /// <summary>
        /// создает файл с настройками по умолчанию
        /// </summary>
        private void createXmlCfg() {
            XmlTextWriter tW = new XmlTextWriter("cfg.xml", null);
            tW.WriteStartDocument();
            ///
            tW.WriteStartElement("SaveData");
            tW.WriteStartElement("SetConnection");
            /// Настройки СОМ-порта
            tW.WriteStartElement("RS_232");
            string[] TagCom = new string[] { 
              "PortName", "Speed"           
            };
            string[] TagComValue = new string[] { 
              "COM1", "9600"           
            };
            for (int i = 0; i < 2; i++)
            {
                tW.WriteStartElement(TagCom[i]);
                tW.WriteString(TagComValue[i]);
                tW.WriteEndElement();
            }
            ///
            tW.WriteEndElement();
            tW.WriteStartElement("Ethernet");
            // настройки сети   
            string[] TagEt = new string[] { 
              "sIP", "sPort" , "dIP", "dPort",
              "SubMask","Gateway" };
            string[] TagEtValue = new string[] { 
              "172.23.132.223", "5000" ,
              "127.0.0.1", "5001",
              "255.255.224.0","172.23.128.254"
            };
            for (int i = 0; i < 6; i++)
            {
                tW.WriteStartElement(TagEt[i]);
                tW.WriteString(TagEtValue[i]); tW.WriteEndElement();
               
            }
              tW.WriteEndElement();  // ethernet
            tW.WriteEndElement();
            // общие настройки
            ///
            string[] TagsNames = new string[] { 
                "OutFrequency",  "OutPower", 
                "AutoRun",       "RegimeMS", 
                "RegimePP",      "PreDistortion",
                "FrequencyDivisionMultiplexing",
                "Output",        "AmplificationAB_dB"};
            string[] Values = new string[] {
                "103.05",        "250",
                "выключить",     "моно", 
                "полярная модуляция",  "выключить",
                "выключить",
                "аналоговый",  "+3.5"};
            tW.WriteStartElement("MainTransiver");
            for (int i = 0; i < 9; i++)
            {
                tW.WriteStartElement(TagsNames[i]);
                tW.WriteString(Values[i]);
                tW.WriteEndElement();
            }
            tW.WriteEndElement(); /// StartElement("Ma..
            ///
            tW.WriteStartElement("GlobalSets");  // общие настройки
            string[] TagsGS = new string[] { 
              "ConnectionType", "Power", "Reserve"
            };
            string[] TagsGSV = new string[] { 
              "Ethernet", "10-500 Вт", "включено"
            };
            for (int i = 0; i < 3; i++) {
                tW.WriteStartElement(TagsGS[i]);
                tW.WriteString(TagsGSV[i]);
                tW.WriteEndElement();
            }
                tW.WriteEndElement();
            ///
            tW.WriteEndElement();
            ///
            tW.WriteEndDocument();
            tW.Close();  
        }
        /// <summary>
        /// загружает занчения из файла либо сохраняет в файл
        /// </summary>
        /// <param name="Load"></param>
        /// <returns></returns>
        private bool loadSaveNetSet(bool Load) {
            bool itog = true;
            XmlDocument xmlDoc;  // заготовка
            xmlDoc = new XmlDocument();  // без конкретизации
            try
            {
                xmlDoc.Load("cfg.xml");
                XmlNodeList xmlNodes;
                XmlNode xmlElement;
                string[] pathSet = new string[3];
                pathSet[0] = "/SaveData/SetConnection/Ethernet/sIP";
                pathSet[1] = "/SaveData/SetConnection/Ethernet/SubMask";
                pathSet[2] = "/SaveData/SetConnection/Ethernet/Gateway";
                string elementValue;
                for (int j = 0; j < 3; j++)
                {
                    xmlNodes = xmlDoc.SelectNodes(pathSet[j]);
                    // Производим циклический перебор найденных элементов,
                    for (int i = 0; i < xmlNodes.Count; i++)
                    {
                        xmlElement = xmlNodes[i];
                        if (xmlElement.HasChildNodes)
                        {
                            // чтение
                            elementValue = xmlElement.FirstChild.Value.Trim();
                            
                            switch (j)
                            {
                                case 0:
                                    {
                                        if (Load) textBoxIP.Text = elementValue;
                                        else xmlElement.InnerText = textBoxIP.Text;
                                    } break;
                                case 1:
                                    {
                                        if (Load) textBoxSM.Text = elementValue;
                                        else xmlElement.InnerText = textBoxSM.Text;
                                    } break; 
                                case 2:
                                    {
                                        if (Load) textBoxGW.Text = elementValue;
                                        else xmlElement.InnerText = textBoxGW.Text;
                                    } break;
                                default: { } break;
                            }
                            
                        }
                    }
                }  // for (int j =..
                if (!Load) xmlDoc.Save("cfg.xml");
            }

            catch (XPathException ex)
            {
                listBox1.Items.Add("Элемент не найден.");
            }
            catch (XmlException err)
            {
                listBox1.Items.Add("Исключения при работе с xml.");
            }
            catch (FileNotFoundException err2)
            {
                listBox1.Items.Add("Ошибка: Файл cfg.xml не найден и создан с настройками");
                listBox1.Items.Add("по умолчанию и данные из него загружены");
                createXmlCfg();
                itog = false;  // нужен еще проход
            }
            return itog;
        }
       
        /// <summary>
        /// OnEvent
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            button2.Enabled = false;
            button4.Enabled = false;
            getNetWorkState(false, false);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            listBox1.Items.Add("Подождите. Смена настрек может занять несколько секунд.");
            getNetWorkState(true, false);  // установить, на форму не выводимть
        }

        private void button3_Click(object sender, EventArgs e)
        {
            listBox1.Items.Clear();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            getNetWorkState(false, true);  // читать и вывести на форму
        }

        private void button5_Click(object sender, EventArgs e)
        {

        }
        private void ClosedForm(object sender, FormClosedEventArgs e)
        {
            loadSaveNetSet(false);  // сохраняем настройки
        }

        private void ClosingForm(object sender, FormClosingEventArgs e)
        {
            loadSaveNetSet(false);  // сохраняем настройки
        }
    }
}
