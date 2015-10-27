#include "Dialog.h"

Dialog *Dialog::getInstance() {
    static Dialog *dlg = nullptr;

    if (!dlg)
        dlg = new Dialog();

    return dlg;
}

Dialog::Dialog() {
	m_layer = load_layer("dialog_scene.csb");
	auto layout = get_layout(m_layer);
	CC_ASSERT(layout);
	m_layer->removeFromParent();

	m_caption = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_caption"));
	m_text_cont = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_content"));
	m_edit_cont = static_cast<TextField *>(Helper::seekWidgetByName(layout, "edit_content"));
	setClickCallback(layout, "button_ok", CC_CALLBACK_1(Dialog::onOkClick, this));
	setClickCallback(layout, "button_cancel", CC_CALLBACK_1(Dialog::onCancelClick, this));
}

void Dialog::onOkClick(Ref *ref) {
	if(m_callback)
        m_callback(this, true);
	Exit();
}

void Dialog::onCancelClick(Ref *ref) {
	if(m_callback)
        m_callback(this, false);
	Exit();
}

void Dialog::Exit() {
    m_layer->retain(); //Retain 一次，防止被自动内存管理器释放
    m_layer->removeFromParent();
    m_callback = nullptr;
}
//弹出对话框
void Dialog::Popup(Node *parent) {
    parent->addChild(m_layer);
}

void Dialog::Popup_t(Node *parent,
    const string& caption,
    const string& content) {
    m_caption->setString(caption);
    m_edit_cont->setVisible(false);
    m_text_cont->setString(content);
    m_text_cont->setVisible(true);
    Popup(parent);
}

void Dialog::Popup_e(Node *parent,
    const string& caption,
    const string& holder) {
    m_caption->setString(caption);
    m_text_cont->setVisible(false);
    m_edit_cont->setString("");
    m_edit_cont->setPlaceHolder(holder);
    m_edit_cont->setVisible(true);
    Popup(parent);
}
