/* -*- C++ -*- */
/* ====================================================================
 * The QuickFIX Software License, Version 1.0
 *
 * Copyright (c) 2001 ThoughtWorks, Inc.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by
 *        ThoughtWorks, Inc. (http://www.thoughtworks.com/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "QuickFIX" and "ThoughtWorks, Inc." must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact quickfix-users@lists.sourceforge.net.
 *
 * 5. Products derived from this software may not be called "QuickFIX",
 *    nor may "QuickFIX" appear in their name, without prior written
 *    permission of ThoughtWorks, Inc.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THOUGHTWORKS INC OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 */

#ifndef FIX_DATADICTIONARY_H
#define FIX_DATADICTIONARY_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Fields.h"
#include "FieldMap.h"
#include "DOMDocument.h"
#include <set>
#include <map>
#include <string.h>

namespace FIX
{
class FieldMap;

class DataDictionary
{
  typedef std::set < int > MsgFields;
  typedef std::map < std::string, MsgFields > MsgTypeToField;
  typedef std::set < std::string > MsgTypes;
  typedef std::set < int > Fields;
  typedef std::map < int, TYPE::Type > FieldTypes;
  typedef std::set < std::string > Values;
  typedef std::map < int, Values > FieldToValue;
  typedef std::map < int, std::string > FieldToName;
  typedef std::map < std::string, int > NameToField;
  typedef std::map < std::pair < int, std::string > ,
  std::string  > ValueToName;
  typedef std::map < std::pair < std::string, int > ,
  std::pair < int, DataDictionary* > > FieldToGroup;

public:
  DataDictionary();
  DataDictionary( const DataDictionary& );
  DataDictionary( const std::string& url );
  virtual ~DataDictionary();
  void readFromURL( const std::string& url );

  // storage functions
  void setVersion( const std::string& beginString )
  {
    m_beginString = beginString;
    m_hasVersion = true;
  }
  std::string getVersion() const
  {
    return m_beginString;
  }

  void addField( int field )
  {
    m_fields.insert( field );
  }

  void addFieldName( int field, const std::string& name )
  {
    m_fieldNames[field] = name;
    m_names[name] = field;
  }

  bool getFieldName( int field, std::string& name ) const
  {
    FieldToName::const_iterator i = m_fieldNames.find( field );
    if(i == m_fieldNames.end()) return false;
    name = i->second;
    return true;
  }

  void addValueName( int field, const std::string& value, const std::string& name )
  {
    m_valueNames[std::make_pair(field, value)] = name;
  }

  bool getValueName( int field, const std::string& value, std::string& name ) const
  {
    ValueToName::const_iterator i = m_valueNames.find( std::make_pair(field, value) );
    if(i == m_valueNames.end()) return false;
    name = i->second;
    return true;
  }

  bool isField( int field ) const
  {
    return m_fields.find( field ) != m_fields.end();
  }

  void addMsgType( const std::string& msgType )
  {
    m_messages.insert( msgType );
  }

  bool isMsgType( const std::string& msgType ) const
  {
    return m_messages.find( msgType ) != m_messages.end();
  }

  void addMsgField( const std::string& msgType, int field )
  {
    m_messageFields[ msgType ].insert( field );
  }

  bool isMsgField( const std::string& msgType, int field ) const
  {
    MsgTypeToField::const_iterator i = m_messageFields.find( msgType );
    if ( i == m_messageFields.end() ) return false;
    return i->second.find( field ) != i->second.end();
  }

  void addHeaderField( int field )
  {
    m_headerFields.insert( field );
  }

  bool isHeaderField( int field ) const
  {
    return m_headerFields.find( field ) != m_headerFields.end();
  }

  void addTrailerField( int field )
  {
    m_trailerFields.insert( field );
  }

  bool isTrailerField( int field ) const
  {
    return m_trailerFields.find( field ) != m_trailerFields.end();
  }

  void addFieldType( int field, TYPE::Type type )
  {
    m_fieldTypes[ field ] = type;
  }

  bool getFieldType( int field, TYPE::Type& type ) const
  {
    FieldTypes::const_iterator i = m_fieldTypes.find( field );
    if ( i == m_fieldTypes.end() ) return false;
    type = i->second;
    return true;
  }

  void addRequiredField( const std::string& msgType, int field )
  {
    m_requiredFields[ msgType ].insert( field );
  }

  bool isRequiredField( const std::string& msgType, int field ) const
  {
    MsgTypeToField::const_iterator i = m_requiredFields.find( msgType );
    if ( i == m_requiredFields.end() ) return false;
    return i->second.find( field ) != i->second.end();
  }

  void addFieldValue( int field, const std::string& value )
  {
    m_fieldValues[ field ].insert( value );
  }

  bool hasFieldValue( int field ) const
  {
    FieldToValue::const_iterator i = m_fieldValues.find( field );
    return i != m_fieldValues.end();
  }

  bool isFieldValue( int field, const std::string& value ) const
  {
    FieldToValue::const_iterator i = m_fieldValues.find( field );
    if ( i == m_fieldValues.end() )
      return false;
    if( !isMultipleValueStringField( field ) )
      return i->second.find( value ) != i->second.end();

    // MultipleValueString
    std::string::size_type startPos = 0;
    std::string::size_type endPos = 0; 
    do
    {
      endPos = value.find_first_of(' ', startPos);
      std::string singleValue =
        value.substr( startPos, endPos - startPos );
      if( i->second.find( singleValue ) == i->second.end() )
        return false;
      startPos = endPos + 1;
    } while( endPos != std::string::npos );
    return true;
  }

  void addGroup( const std::string& msg, int field, int delim,
                 const DataDictionary& dataDictionary )
  {
    DataDictionary * pDD = new DataDictionary;
    *pDD = dataDictionary;
    pDD->setVersion( getVersion() );
    m_groups[ std::make_pair( msg, field ) ] = std::make_pair( delim, pDD );
  }

  bool isGroup( const std::string& msg, int field ) const
  {
    return m_groups.find( std::make_pair( msg, field ) ) != m_groups.end();
  }

  bool getGroup( const std::string& msg, int field, int& delim,
                 const DataDictionary*& pDataDictionary ) const
  {
    FieldToGroup::const_iterator i =
      m_groups.find( std::make_pair( msg, field ) );
    if ( i == m_groups.end() ) return false;
    std::pair < int, DataDictionary* > pair = i->second;
    delim = pair.first;
    pDataDictionary = pair.second;
    return true;
  }

  bool isDataField( int field ) const
  {
    FieldTypes::const_iterator i = m_fieldTypes.find( field );
    return i != m_fieldTypes.end() && i->second == TYPE::Data;
  }

  bool isMultipleValueStringField( int field ) const
  {
    FieldTypes::const_iterator i = m_fieldTypes.find( field );
    return i != m_fieldTypes.end() && i->second == TYPE::MultipleValueString;
  }

  void checkFieldsOutOfOrder( bool value )
  { m_checkFieldsOutOfOrder = value; }
  void checkFieldsHaveValues( bool value )
  { m_checkFieldsHaveValues = value; }

  /// Validate a message.
  void validate( const Message& message ) throw( std::exception& );
  DataDictionary& operator=( const DataDictionary& rhs );

private:
  /// Iterate through fields while applying checks.
  void iterate( const FieldMap& map, const MsgType& msgType );

  /// Check if message type is defined in spec.
  void checkMsgType( const MsgType& msgType ) const
  {
    if ( !isMsgType( msgType.getValue() ) )
      throw InvalidMessageType();
  }

  /// Check if field tag number is defined in spec.
  void checkValidTagNumber( const FieldBase& field ) const
  throw( InvalidTagNumber& )
  {
    if( m_fields.find( field.getField() ) == m_fields.end() )
      throw InvalidTagNumber( field.getField() );
  }

  void checkValidFormat( const FieldBase& field ) const
  throw( IncorrectDataFormat& )
  {
    try
    {
      TYPE::Type type = TYPE::Unknown;
      getFieldType( field.getField(), type );
      switch ( type )
      {
      case TYPE::String:
        STRING_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Char:
        CHAR_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Price:
        PRICE_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Int:
        INT_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Amt:
        AMT_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Qty:
        QTY_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Currency:
        CURRENCY_CONVERTOR::convert( field.getString() ); break;
      case TYPE::MultipleValueString:
        MULTIPLEVALUESTRING_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Exchange:
        EXCHANGE_CONVERTOR::convert( field.getString() ); break;
      case TYPE::UtcTimeStamp:
        UTCTIMESTAMP_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Boolean:
        BOOLEAN_CONVERTOR::convert( field.getString() ); break;
      case TYPE::LocalMktDate:
        LOCALMKTDATE_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Data:
        DATA_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Float:
        FLOAT_CONVERTOR::convert( field.getString() ); break;
      case TYPE::PriceOffset:
        PRICEOFFSET_CONVERTOR::convert( field.getString() ); break;
      case TYPE::MonthYear:
        MONTHYEAR_CONVERTOR::convert( field.getString() ); break;
      case TYPE::DayOfMonth:
        DAYOFMONTH_CONVERTOR::convert( field.getString() ); break;
      case TYPE::UtcDate:
        UTCDATE_CONVERTOR::convert( field.getString() ); break;
      case TYPE::UtcTimeOnly:
        UTCTIMEONLY_CONVERTOR::convert( field.getString() ); break;
      case TYPE::NumInGroup:
        NUMINGROUP_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Percentage:
        PERCENTAGE_CONVERTOR::convert( field.getString() ); break;
      case TYPE::SeqNum:
        SEQNUM_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Length:
        LENGTH_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Country:
        COUNTRY_CONVERTOR::convert( field.getString() ); break;
      case TYPE::Unknown: break;
      }
    }
    catch ( FieldConvertError& )
    { throw IncorrectDataFormat( field.getField() ); }
  }

  void checkValue( const FieldBase& field ) const
  throw( IncorrectTagValue& )
  {
    if ( !hasFieldValue( field.getField() ) ) return ;

    const std::string& value = field.getString();
    if ( !isFieldValue( field.getField(), value ) )
      throw IncorrectTagValue( field.getField() );
  }

  /// Check if a field has a value.
  void checkHasValue( const FieldBase& field ) const
  throw( NoTagValue& )
  {
    if ( m_checkFieldsHaveValues && !field.getString().length() )
      throw NoTagValue( field.getField() );
  }

  /// Check if a field is in this message type.
  void checkIsInMessage
  ( const FieldBase& field, const MsgType& msgType ) const
  throw( TagNotDefinedForMessage& )
  {
    if ( !isMsgField( msgType, field.getField() ) )
      throw TagNotDefinedForMessage( field.getField() );
  }

  /// Check if group count matches number of groups in
  void checkGroupCount
  ( const FieldBase& field, const FieldMap& fieldMap, const MsgType& msgType ) const
  throw( RepeatingGroupCountMismatch& )
  {
    int fieldNum = field.getField();
    if( isGroup(msgType, fieldNum) )
    {
      if( fieldMap.groupCount(fieldNum) 
        != IntConvertor::convert(field.getString()) )
      throw RepeatingGroupCountMismatch(fieldNum);
    }
  }

  /// Check if a message has all required fields.
  void checkHasRequired
  ( const FieldMap& fieldMap, const MsgType& msgType ) const
  throw( RequiredTagMissing& )
  {
    MsgTypeToField::const_iterator iM
    = m_requiredFields.find( msgType.getValue() );
    if ( iM == m_requiredFields.end() ) return ;

    const MsgFields& fields = iM->second;
    MsgFields::const_iterator iF;
    for ( iF = fields.begin(); iF != fields.end(); ++iF )
    {
      if ( !fieldMap.isSetField( *iF ) )
        throw RequiredTagMissing( *iF );
    }
  }

  /// Read XML file using MSXML.
  void readMSXMLDOM( const std::string& );
  void readMSXML( const std::string& );
  /// Read XML file using libXML.
  void readLibXml( const std::string& );

  int lookupXMLFieldNumber( DOMDocument*, DOMNode* ) const;
  int lookupXMLFieldNumber( DOMDocument*, const std::string& name ) const;
  int addXMLComponentFields( DOMDocument*, DOMNode*, const std::string& msgtype, DataDictionary&, bool );
  void addXMLGroup( DOMDocument*, DOMNode*, const std::string& msgtype, DataDictionary& );
  TYPE::Type XMLTypeToType( const std::string& xmlType ) const;

  bool m_hasVersion;
  bool m_checkFieldsOutOfOrder;
  bool m_checkFieldsHaveValues;
  BeginString m_beginString;
  MsgTypeToField m_messageFields;
  MsgTypeToField m_requiredFields;
  MsgTypes m_messages;
  Fields m_fields;
  Fields m_headerFields;
  Fields m_trailerFields;
  FieldTypes m_fieldTypes;
  FieldToValue m_fieldValues;
  FieldToName m_fieldNames;
  NameToField m_names;
  ValueToName m_valueNames;
  FieldToGroup m_groups;
};
}

#endif //FIX_DATADICTIONARY_H
