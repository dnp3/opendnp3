package com.automatak.render.dnp3.objects.generators

object GroupVariationIncludes {

   def headerReadWrite : Iterator[String] = Iterator(
     "<openpal/container/ReadBufferView.h>",
     "<openpal/container/WriteBufferView.h>",
     """"opendnp3/Types.h""""
   )

  def implReadWrite : Iterator[String] = Iterator(
    "<openpal/serialization/Format.h>",
    "<openpal/serialization/Parse.h>"
  )

 }
