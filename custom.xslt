<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:m="http://www.w3.org/1998/Math/MathML"
                xmlns:mal="http://projectmallard.org/1.0/"
                version="1.0">

  <xsl:template mode="mal2html.inline.mode" match="m:*">
    <xsl:element name="{local-name()}" namespace="http://www.w3.org/1998/Math/MathML">
      <xsl:apply-templates mode="mal2html.inline.mode" select="@*|node()"/>
    </xsl:element>
  </xsl:template>

  <xsl:template name="html.head.custom">
    <link rel="stylesheet" href="../index.css" type="text/css"/>
  </xsl:template>

  <xsl:template name="html.top.custom">
    <xsl:param name="node"/>

    <!-- top banner -->
    <div class="banner">
      <h1 class="banner"><a href="#">
        <span>Advanced GTK Techniques</span></a>
      </h1>
      <h2 class="banner">
        <span><xsl:value-of select="$node/mal:title"/></span>
      </h2>
    </div>

    <!-- Custom prev/next links -->
    <xsl:call-template name="my-custom-prev-next-links">
      <xsl:with-param name="node" select="$node"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="html.bottom.custom">
    <xsl:param name="node"/>
    <!-- Repeat the custom prev/next links at the bottom -->
    <xsl:call-template name="my-custom-prev-next-links">
      <xsl:with-param name="node" select="$node"/>
    </xsl:call-template>
  </xsl:template>

  <!-- Template based on mal2html.links.prevnext -->
  <xsl:template name="my-custom-prev-next-links">
    <xsl:param name="node"/>

    <xsl:variable name="page" select="$node/ancestor-or-self::mal:page[last()]"/>
    <xsl:variable name="linkid">
      <xsl:call-template name="mal.link.linkid">
        <xsl:with-param name="node" select="$page"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="next" select="$page/mal:info/mal:link[@type='next']"/>
    <xsl:for-each select="$mal.cache">
      <xsl:variable name="prev" select="key('mal.cache.link.key', concat('next:', $linkid))"/>
      <div class="links nextlinks">
        <xsl:if test="$prev">
          <xsl:text>« </xsl:text>
          <a class="nextlinks-prev">
            <xsl:attribute name="href">
              <xsl:call-template name="mal.link.target">
                <xsl:with-param name="node" select="$prev"/>
                <xsl:with-param name="xref" select="$prev/../../@id"/>
              </xsl:call-template>
            </xsl:attribute>
            <xsl:attribute name="title">
              <xsl:call-template name="mal.link.tooltip">
                <xsl:with-param name="node" select="$prev"/>
                <xsl:with-param name="xref" select="$prev/../../@id"/>
              </xsl:call-template>
            </xsl:attribute>
            <xsl:call-template name="mal.link.content">
              <xsl:with-param name="node" select="$prev"/>
              <xsl:with-param name="xref" select="$prev/../../@id"/>
              <xsl:with-param name="role" select="'trail'"/>
            </xsl:call-template>
          </a>
          <xsl:text> :: </xsl:text>
        </xsl:if>
        <xsl:variable name="contents" select="$page/mal:info/mal:link[@type='guide']"/>
        <a class="nextlinks-contents">
          <xsl:attribute name="href">
            <xsl:call-template name="mal.link.target">
              <xsl:with-param name="node" select="$contents"/>
              <xsl:with-param name="xref" select="$contents/@xref"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="title">
            <xsl:call-template name="mal.link.tooltip">
              <xsl:with-param name="node" select="$contents"/>
              <xsl:with-param name="xref" select="$contents/@xref"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:call-template name="l10n.gettext">
            <xsl:with-param name="msgid" select="'Contents'"/>
          </xsl:call-template>
        </a>
        <xsl:if test="$next">
          <xsl:text> :: </xsl:text>
          <a class="nextlinks-next">
            <xsl:attribute name="href">
              <xsl:call-template name="mal.link.target">
                <xsl:with-param name="node" select="$next"/>
                <xsl:with-param name="xref" select="$next/@xref"/>
              </xsl:call-template>
            </xsl:attribute>
            <xsl:attribute name="title">
              <xsl:call-template name="mal.link.tooltip">
                <xsl:with-param name="node" select="$next"/>
                <xsl:with-param name="xref" select="$next/@xref"/>
              </xsl:call-template>
            </xsl:attribute>
            <xsl:call-template name="mal.link.content">
              <xsl:with-param name="node" select="$next"/>
              <xsl:with-param name="xref" select="$next/@xref"/>
              <xsl:with-param name="role" select="'trail'"/>
            </xsl:call-template>
          </a>
          <xsl:text> »</xsl:text>
        </xsl:if>
      </div>
    </xsl:for-each>
  </xsl:template>

  <!-- Override Mallard's built-in previous/next links to be blank -->
  <xsl:template name="mal2html.links.prevnext">
    <xsl:param name="node" select="."/>
  </xsl:template>

</xsl:stylesheet>

