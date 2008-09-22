#!/usr/bin/ruby -KU

#
# Usage : bin/convertComments2OtherLanguage.rb NihongoShoho01.xml > NihongoShoho01v2.xml
#

require 'rexml/document'
include REXML

filename = ARGV[ 0 ]
otherLang = ARGV[ 1 ]

xml = REXML::Document.new( File.open( filename ) )
xml.elements.each( '//term' ) do 
    | term |
    comment = term.elements[ 'comment' ]
    if( comment )
        commentText = comment.text
        if( commentText.length > 0 )
            # Make new trans element.
            trans = Element.new( 'trans' )
            trans.attributes[ 'lang' ] = otherLang
            word = Element.new( 'word' )
            word.text = commentText
            trans.elements[ 1 ] = word

            # Remove obsolete comment element.
            term.delete_element term.elements[ 'comment' ]

            # Add new trans element.
            term.elements[ 3 ] = trans
        end
    end
end

xml.write( STDOUT, 0 )
exit
