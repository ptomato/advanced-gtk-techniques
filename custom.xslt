<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
               xmlns:m="http://www.w3.org/1998/Math/MathML"
               version="1.0">
  <xsl:template mode="mal2html.inline.mode" match="m:*">
    <xsl:element name="{local-name()}" namespace="http://www.w3.org/1998/Math/MathML">
      <xsl:apply-templates mode="mal2html.inline.mode" select="@*|node()"/>
    </xsl:element>
  </xsl:template>

  <xsl:template name="html.head.custom">
    <link rel="stylesheet" href="../index.css" type="text/css"/>
  </xsl:template>

</xsl:stylesheet>

