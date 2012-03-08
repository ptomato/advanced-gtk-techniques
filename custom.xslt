<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:m="http://www.w3.org/1998/Math/MathML"
                xmlns:mal="http://projectmallard.org/1.0/"
                version="1.0">

  <!-- Parameters -->
  <xsl:param name="mal2html.editor_mode" select="true()"/>
  <xsl:param name="html.syntax.highlight" select="false()"/>

  <!-- Pass MathML elements through -->
  <xsl:template mode="mal2html.inline.mode" match="m:*">
    <xsl:element name="{local-name()}" namespace="http://www.w3.org/1998/Math/MathML">
      <xsl:apply-templates mode="mal2html.inline.mode" select="@*|node()"/>
    </xsl:element>
  </xsl:template>

  <xsl:template name="html.head.custom">
    <link rel="stylesheet" href="../index.css" type="text/css"/>
    <script src="../highlight.pack.js"/>
    <script>
$(document).ready(function() {
  $('div.code pre').each(function(i, e) {hljs.highlightBlock(e, '    ')});
});
    </script>
    <link rel="stylesheet" href="http://yandex.st/highlightjs/6.1/styles/github.min.css"/>
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

  <!-- Override Mallard's <pre> template to provide a starting line number
  and highlighting class attributes for highlight.js -->
  <xsl:template name="mal2html.pre">
  <xsl:param name="node" select="."/>
  <xsl:param name="numbered" select="contains(concat(' ', @style, ' '), 'numbered')"/>
  <xsl:variable name="if"><xsl:call-template name="mal.if.test"/></xsl:variable><xsl:if test="$if != ''">
  <xsl:variable name="first" select="$node/node()[1]/self::text()"/>
  <xsl:variable name="last" select="$node/node()[last()]/self::text()"/>
  <div>
    <xsl:call-template name="html.lang.attrs">
      <xsl:with-param name="node" select="$node"/>
    </xsl:call-template>
    <xsl:attribute name="class">
      <xsl:value-of select="local-name($node)"/>
      <xsl:if test="$if != 'true'">
        <xsl:text> if-if </xsl:text>
        <xsl:value-of select="$if"/>
      </xsl:if>
    </xsl:attribute>
    <xsl:if test="$numbered">
      <pre class="numbered"><xsl:call-template name="utils.linenumbering">
        <xsl:with-param name="node" select="$node"/>
        <xsl:with-param name="number" select="@startline"/> <!-- PC -->
      </xsl:call-template></pre>
    </xsl:if>
    <pre>
      <xsl:attribute name="class">
        <xsl:text>contents </xsl:text>
        <xsl:if test="$node/@mime">
          <xsl:choose>
            <xsl:when test="@mime = 'application/x-shellscript'">
              <xsl:text>language-bash</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/x-csrc' or @mime = 'text/x-chdr' or
                            @mime = 'text/x-c++hdr' or @mime = 'text/x-c++src'">
              <xsl:text>language-cpp</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/x-objcsrc'">
              <xsl:text>language-objectivec</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/x-csharp'">
              <xsl:text>language-cs</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/css'">
              <xsl:text>language-css</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/x-patch'">
              <xsl:text>language-diff</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/html' or @mime = 'application/xml' or
                            substring(@mime, string-length(@mime) - 3) = '+xml'">
              <xsl:text>language-xml</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/x-java'">
              <xsl:text>language-java</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'application/javascript'">
              <xsl:text>language-javascript</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/x-scheme' or @mime = 'text/x-emacs-lisp'">
              <xsl:text>language-lisp</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/x-lua'">
              <xsl:text>language-lua</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/x-pascal'">
              <xsl:text>language-pascal</xsl:text> <!-- Not supported highlight.js -->
            </xsl:when>
            <xsl:when test="@mime = 'application/x-perl'">
              <xsl:text>language-perl</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'application/x-php'">
              <xsl:text>language-php</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/x-python'">
              <xsl:text>language-python</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'application/x-ruby'">
              <xsl:text>language-ruby</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'text/x-sql'">
              <xsl:text>language-sql</xsl:text>
            </xsl:when>
            <xsl:when test="@mime = 'application/x-yaml'">
              <xsl:text>language-yaml</xsl:text> <!-- Not supported highlight.js -->
            </xsl:when>
            <xsl:when test="@mime = 'text/x-desktop'">
              <xsl:text>language-ini</xsl:text>
            </xsl:when>
          </xsl:choose>
        </xsl:if>
      </xsl:attribute>
      <xsl:if test="$first">
        <xsl:call-template name="utils.strip_newlines">
          <xsl:with-param name="string" select="$first"/>
          <xsl:with-param name="leading" select="true()"/>
          <xsl:with-param name="trailing" select="count(node()) = 1"/>
        </xsl:call-template>
      </xsl:if>
      <xsl:apply-templates mode="mal2html.inline.mode"
                           select="node()[not(self::text() and (position() = 1 or position() = last()))]"/>
      <xsl:if test="$last and (count(node()) != 1)">
        <xsl:call-template name="utils.strip_newlines">
          <xsl:with-param name="string" select="$last"/>
          <xsl:with-param name="leading" select="false()"/>
          <xsl:with-param name="trailing" select="true()"/>
        </xsl:call-template>
      </xsl:if>
    </pre>
  </div>
</xsl:if>
</xsl:template>

</xsl:stylesheet>

