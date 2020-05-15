/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.dnp3.codegen

import java.lang.reflect.{Constructor, Method, Field}

object JNIMethod {

  val classOfBool : Class[_] = classOf[java.lang.Boolean]
  val classOfShort : Class[_] = classOf[java.lang.Short]
  val classOfInt : Class[_] = classOf[java.lang.Integer]
  val classOfLong : Class[_] = classOf[java.lang.Long]
  val classOfFloat : Class[_] = classOf[java.lang.Float]
  val classOfDouble : Class[_] = classOf[java.lang.Double]
  val classOfString : Class[_] = classOf[java.lang.String]

  def getType(clazz: Class[_]): String = {

    def getPrivitiveType : String = clazz.getTypeName match {
      case "void" => "void"
      case "boolean" => "jboolean"
      case "int" => "jint"
      case "byte" => "jbyte"
      case "long" => "jlong"
      case "float" => "jfloat"
      case "short" => "jshort"
      case "double" => "jdouble"
      case _ => throw new Exception("undefined primitive type: %s".format(clazz.getTypeName))
    }

    if (clazz.isPrimitive) getPrivitiveType else clazz.wrapperName

  }

  def getFieldType(clazz: Class[_]): String = clazz match {

    case `classOfInt` => "I"
    case `classOfLong` => "J"
    case `classOfBool` => "Z"
    case `classOfDouble` => "D"
    case _ => {
      if(clazz.isPrimitive) {
        clazz.getTypeName match {
          case "boolean" => "Z"
          case "int" => "I"
          case "void" => "void"
          case "byte" => "B"
          case "long" => "J"
          case "short" => "S"
          case "float" => "F"
          case "double" => "D"
          case _ => throw new Exception("undefined primitive type: %s".format(clazz.getTypeName))
        }
      } else {
        clazz.fqcn
      }

    }
  }

  def getReturnType(clazz: Class[_]): String = clazz match {

    case `classOfInt` => "Int"
    case `classOfLong` => "Long"
    case `classOfBool` => "Boolean"
    case `classOfDouble` => "Double"
    case _ => {
      if(clazz.isPrimitive) {
        clazz.getTypeName match {
          case "boolean" => "Boolean"
          case "int" => "Int"
          case "long" => "Long"
          case "void" => "Void"
          case "byte" => "Byte"
          case "short" => "Short"
          case "float" => "Float"
          case "double" => "Double"
          case _ => throw new Exception("undefined primitive type: %s".format(clazz.getTypeName))
        }
      } else {
        "Object"
      }
    }
  }

  def getSignature(method: Method, className: Option[String] = None) : String = {

    def returnType: String = {
      if(method.getReturnType.isPrimitive) {
        getType(method.getReturnType)
      }
      else {
        s"LocalRef<${getType(method.getReturnType)}>"
      }
    }

    def arguments: String = {
      if(method.getParameterCount == 0) "" else {
        ", " + method.getParameters.map(p => s"${getType(p.getType)} ${p.getName}").mkString(", ")
      }
    }

    def prefix = className.map(n => "%s::".format(n)).getOrElse("")

    def additionalArgs = if(method.isStatic) "" else s", ${getType(method.getDeclaringClass)} instance"

    "%s %s%s(JNIEnv* env%s%s)".format(returnType, prefix, method.getName, additionalArgs, arguments)
  }

  def getImpl(id : Int, method: Method)(implicit i: Indentation) : Iterator[String] = {

    def returnPrefix : String = {

      if(method.getReturnType.isPrimitive)
      {
        if(method.isVoid) "" else "return "
      }
      else
      {
        s"return LocalRef<${getType(method.getReturnType)}>(env, "
      }

    }

    def returnSuffix : String = {

      if(!method.getReturnType.isPrimitive)
      {
        ");"
      }
      else
      {
        ";"
      }

    }

    def args : String = if(method.getParameterCount == 0) "" else {
      ", " + method.getParameters.map(p => p.getName).mkString(", ")
    }

    def callMethod : String = {
      if(method.isStatic) {
        "%senv->CallStatic%sMethod(this->clazz, this->method%d%s)%s".format(
          returnPrefix,
          getReturnType(method.getReturnType),
          id,
          args,
          returnSuffix
        )
      }
      else {
        "%senv->Call%sMethod(instance, this->method%d%s)%s".format(
          returnPrefix,
          getReturnType(method.getReturnType),
          id,
          args,
          returnSuffix
        )
      }
    }

    JNIMethod.getSignature(method, Some(method.getDeclaringClass.getSimpleName)).iter ++ bracket {
      callMethod.iter
    }

  }

  def getConstructorSignature(constructor: Constructor[_], className: Option[String] = None) : String = {


    def arguments = constructor.getParameters.map(p => "%s %s".format(getType(p.getType), p.getName)).mkString(", ")

    val wrapperName = constructor.getDeclaringClass.wrapperName

    if(arguments.isEmpty) {
      "LocalRef<%s> %sconstruct(JNIEnv* env)".format(wrapperName, className.map(n => "%s::".format(n)).getOrElse(""))
    }
    else {
      "LocalRef<%s> %sconstruct(JNIEnv* env, %s)".format(wrapperName, className.map(n => "%s::".format(n)).getOrElse(""), arguments)
    }

  }

  def getConstructorImpl(id: Int, constructor: Constructor[_])(implicit i: Indentation) : Iterator[String] = {

    def args : String = if(constructor.getParameterCount == 0) "" else {
      ", " + constructor.getParameters.map(p => p.getName).mkString(", ")
    }

    val wrapperName = constructor.getDeclaringClass.wrapperName

    JNIMethod.getConstructorSignature(constructor, Some(constructor.getDeclaringClass.getSimpleName)).iter ++ bracket {
      s"return LocalRef<$wrapperName>(env, $wrapperName(env->NewObject(this->clazz, this->constructor$id$args)));".iter
    }

  }

  def getFieldGetterImpl(f : Field)(implicit i: Indentation) : Iterator[String] = {

    def fieldType : String = getReturnType(f.getType)

    def cast : String = if(f.getType == classOf[String]) "(jstring) " else ""

    val wrapperName = f.getDeclaringClass.wrapperName

    if(f.getType.isPrimitive) {
      s"${getType(f.getType)} ${f.getDeclaringClass.getSimpleName}::get${f.getName}(JNIEnv* env, $wrapperName instance)".iter ++ bracket {
        "return %senv->Get%sField(instance, this->%sField);".format(cast, fieldType, f.getName).iter
      }
    }
    else {
      s"LocalRef<${getType(f.getType)}> ${f.getDeclaringClass.getSimpleName}::get${f.getName}(JNIEnv* env, $wrapperName instance)".iter  ++ bracket {
        s"return LocalRef<${getType(f.getType)}>(env, ${cast}env->Get${fieldType}Field(instance, this->${f.getName}Field));".iter
      }
    }


  }
}
