#include "doxysampleclass.h"
/*!
 * \brief Creates a new DoxySampleClass object
 * \param parent
 */
DoxySampleClass::DoxySampleClass(QObject *parent) :
    QObject(parent)
{
}

/*!
 * \brief Destructor
 */
DoxySampleClass::~DoxySampleClass()
{
}

/*!
 * \brief DoxySampleClass::propExample
 * \return Returns the current propExample value
 */
double DoxySampleClass::propExample() const
{
    return 0.0;
}

/*!
 * \brief DoxySampleClass::publicStaticExample
 */
void DoxySampleClass::publicStaticExample()
{
}

/*!
 * \brief DoxySampleClass::setPropExample
 * \param val
 */
void DoxySampleClass::setPropExample(double val)
{
    propVal = val;
}

/*!
 * \brief DoxySampleClass::event
 * \param ev
 */
bool DoxySampleClass::event(QEvent *ev)
{
    return QObject::event(ev);
}

/*!
 * \brief DoxySampleClass::protectedExample
 */
void DoxySampleClass::protectedExample()
{
}

/*!
 * \brief DoxySampleClass::protectedStaticExample
 */
void DoxySampleClass::protectedStaticExample()
{
}

/*!
 * \brief DoxySampleClass::protectedSlotExample
 */
void DoxySampleClass::protectedSlotExample()
{
}

/*!
 * \brief DoxySampleClass::privateExample
 */
void DoxySampleClass::privateExample()
{
}

/*!
 * \brief DoxySampleClass::privateStaticExample
 */
void DoxySampleClass::privateStaticExample()
{

}
/*!
 * \brief Something something
 * \return \c true if the object is running; otherwise returns \c false.
 */
bool DoxySampleClass::isRunning() const
{
    return false;
}

/*!
 * \brief Something something
 * \return \c true if the object is finished; otherwise returns \c false.
 */
bool DoxySampleClass::isFinished() const
{
    return false;
}
